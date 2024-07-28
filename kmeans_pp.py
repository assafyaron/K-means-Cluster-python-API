import math
import sys
import pandas as pd
import numpy as np
np.random.seed(1234)

# Assert correct values for k, iter, epsilon
def assert_inputs(k, iter, N, eps):
    ok = True
    if not ((1 < k < N)  and (k % 1 == 0)):
        print("Invalid number of clusters!")
        return not ok
    if not ((1 <= iter <= 1000) and (iter % 1 == 0)):
        print("Invalid maximum iteration!")
        return not ok
    if not 0 <= eps <= 1:
        print("Invalid epsilon!")
        return not ok
    return ok

def  create_dict(vectors):
    dict_vectors = {}
    for i in range(len(vectors)):
        dict_vectors[i] = vectors.iloc[i]
    return dict_vectors

def kmeans_pp(vectors, k, N, iter, eps):

    # Initialize centroids
    centroids = []

    # Append all vectors to a new dictionary using their index as key
    dict_vectors = create_dict(vectors)

    print(dict_vectors)

    return 1


def main():

    # Get data from console
    input_data = sys.argv
    if len(input_data) < 6:
        k, iter, eps, file_name1, file_name2 = float(input_data[1]), 300, float(input_data[2]), input_data[3], input_data[4]
    else:
        k, iter, eps, file_name1, file_name2 = float(input_data[1]), float(input_data[2]), float(input_data[3]), input_data[4], input_data[5]

    # Combine both input files into one dataframe using inner-join with the first column as key
    df1 = pd.read_csv(file_name1, header=None)
    df2 = pd.read_csv(file_name2, header=None)
    vectors = pd.merge(df1, df2, on=0, how='inner')

    # Save the result to a new file
    vectors.to_csv('output.csv', header=False, index=False)

    # Sort result by the first column
    vectors = vectors.sort_values(by=[0])

    N = int(len(vectors))

    # Assert inputs
    if not assert_inputs(k, iter, eps, N):
        exit(1)
    
    kmeans_pp(vectors, k, N, iter, eps)

if __name__ == "__main__":
    main()
