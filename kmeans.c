#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cap.h"

void zero_clusters(double **clusters, int k, int vecdim)
{
    int i,j;
    for (i=0;i<k;i++)
    {
        for (j=0;j<vecdim;j++)
        {
            clusters[i][j] = 0;
        }
    }
}

double euclidean_distance(double *vec1, double *vec2, int vecdim)
{
    int i;
    double sum = 0;
    for (i=0;i<vecdim;i++)
    {
        sum += pow(vec1[i] - vec2[i], 2);
    }
    return sqrt(sum);
}

int find_closest_centroid(double *vec, double **centroids, int k, int vecdim)
{
    int i;
    double dist;
    double min_dist = euclidean_distance(vec, centroids[0], vecdim);
    int min_index = 0;
    for (i=1;i<k;i++)
    {
        dist = euclidean_distance(vec, centroids[i], vecdim);
        if (dist < min_dist)
        {
            min_dist = dist;
            min_index = i;
        }
    }
    return min_index;
}

void add_vec_to_cluster(double *vec, double *cluster, int vecdim)
{
    int i;
    for (i=0;i<vecdim;i++)
    {
        cluster[i] += vec[i];
    }
}

void divide_cluster(double *cluster, int vecdim, int k)
{
    int i;
    for (i=0;i<vecdim;i++)
    {
        cluster[i] /= k;
    }
}

void divide_all_clusters(double **clusters, int k, int vecdim, int *cluster_sizes)
{
    int i;
    for (i=0;i<k;i++)
    {
        if (cluster_sizes[i])
        {
            divide_cluster(clusters[i], vecdim, cluster_sizes[i]);
        }
    }
}

int check_convergence(double **centroids, double **clusters, int k, int vecdim)
{
    int i;
    int flag = 1;
    for (i=0;i<k;i++)
    {
        if (!(euclidean_distance(centroids[i], clusters[i], vecdim) < 0.0001))
        {
            flag = 0;
            break;
        }
    }
    return flag;
}

void copy_clusters_to_centroids(double **clusters, double **centroids, int k, int vecdim)
{
    int i,j;
    for (i=0;i<k;i++)
    {
        for (j=0;j<vecdim;j++)
        {
            centroids[i][j] = clusters[i][j];
        }
    }
    zero_clusters(clusters, k, vecdim);
}

/* print a 2D array of doubles with only 4 digits after the point */
void print_vec_arr(double **vec_arr, int N, int vecdim)
{
    int i,j;
    for (i=0;i<N;i++)
    {
        for (j=0;j<vecdim;j++)
        {
            printf("%.4f", vec_arr[i][j]);
            if (j != vecdim-1)
            {
                printf(",");
            }
        }
        printf("\n");
    }
}

void zero_cluster_sizes(int *cluster_sizes, int k)
{
    int i;
    for (i=0;i<k;i++)
    {
        cluster_sizes[i] = 0;
    }
}

void matrix_free(double **p, int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        free(p[i]);
    }
    free(p); 
}

int isNaturalNumber(char *number)
{
    int i = 0;
    int after_point = 0;
    while(number[i]!='\0')
    {
        if ((number[i] != '0') && after_point)
        {
            return 0;
        }
        if (number[i] == '.')
        {
            after_point = 1;
        }
        i++;
    }
    return 1;
}

void kmeans(int k, int N, int vecdim, int iter, double eps, double **vec_arr, double **centroids)
{
    int i,j;

    /* Define clusters cluster_sizes */
    double **clusters;
    int *cluster_sizes;
    

    /* create clusters and cluster_sizes arrays */
    clusters = malloc(k * sizeof(double*));
    if (clusters == NULL)
    {
        printf("An Error Has Occured");
        matrix_free(vec_arr, N);
        matrix_free(clusters, k);
        matrix_free(centroids, k);
        return 1;
    }
    
    cluster_sizes = malloc(k * sizeof(int));
    if (cluster_sizes == NULL)
    {
        printf("An Error Has Occured");
        matrix_free(vec_arr, N);
        matrix_free(clusters, k);
        matrix_free(centroids, k);
        free(cluster_sizes);
        return 1;
    }

    /* zero out the cluster_sizes array */
    zero_cluster_sizes(cluster_sizes, k);

    /* initialize and zero out the clusters array */
    for (i=0;i<k;i++)
    {
        clusters[i] = malloc(vecdim * sizeof(double));
        if (clusters[i] == NULL)
        {
            printf("An Error Has Occured");
            matrix_free(vec_arr, N);
            matrix_free(clusters, k);
            matrix_free(centroids, k);
            free(cluster_sizes);
            return 1;
        }
        for (j=0;j<vecdim;j++)
        {
            clusters[i][j] = 0;
        }
    }

    /* start the k-means algorithm */
    for (i=0;i<iter;i++)
    {
        /* iterate over all vectors */
        for (j=0;j<N;j++)
        {

            /* find the closest centroid */
            int closest_centroid = find_closest_centroid(vec_arr[j], centroids, k, vecdim);
            cluster_sizes[closest_centroid]++;

            /* add the vector to the closest cluster */
            add_vec_to_cluster(vec_arr[j], clusters[closest_centroid], vecdim);
        }

        /* divide all clusters by the number of vectors in them */
        divide_all_clusters(clusters, k, vecdim, cluster_sizes);

        /* check for convergence */
        if (check_convergence(centroids, clusters, k, vecdim))
        {
            break;
        }
        else
        {
            copy_clusters_to_centroids(clusters, centroids, k, vecdim);
            zero_cluster_sizes(cluster_sizes, k);
        }
    }

    print_vec_arr(centroids, k, vecdim);
        
    matrix_free(vec_arr, N);
    matrix_free(clusters, k);
    matrix_free(centroids, k);
    free(cluster_sizes);

    return 0;
}