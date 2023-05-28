#include <stdio.h>
#include <math.h>

#define MAX_DATA_POINTS 100
#define MAX_CLUSTER_POINTS 10
#define MAX_ITERATIONS 100
#define EPSILON 0.0001

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point center;
    double membership;
} ClusterPoint;

double calculateDistance(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void fuzzyCMeans(Point dataPoints[], int numDataPoints, ClusterPoint clusterPoints[], int numClusterPoints) {
    double membershipMatrix[MAX_DATA_POINTS][MAX_CLUSTER_POINTS];
    double oldMembershipMatrix[MAX_DATA_POINTS][MAX_CLUSTER_POINTS];
    double newCenters[MAX_CLUSTER_POINTS][2];
    double oldCenters[MAX_CLUSTER_POINTS][2];
    int iteration = 0;
    int i, j;

    // Initialize membership matrix and cluster centers randomly
    for (i = 0; i < numDataPoints; i++) {
        double membershipSum = 0.0;
        for (j = 0; j < numClusterPoints; j++) {
            membershipMatrix[i][j] = (double)rand() / RAND_MAX;
            membershipSum += membershipMatrix[i][j];
        }
        for (j = 0; j < numClusterPoints; j++) {
            membershipMatrix[i][j] /= membershipSum;
        }
    }

    for (i = 0; i < numClusterPoints; i++) {
        clusterPoints[i].center = dataPoints[rand() % numDataPoints];
        clusterPoints[i].membership = 0.0;
    }

    // Iterate until convergence or maximum iterations reached
    while (iteration < MAX_ITERATIONS) {
        // Update cluster centers
        for (i = 0; i < numClusterPoints; i++) {
            double xSum = 0.0;
            double ySum = 0.0;
            double membershipSum = 0.0;

            for (j = 0; j < numDataPoints; j++) {
                double membershipPower = pow(membershipMatrix[j][i], 2);
                xSum += membershipPower * dataPoints[j].x;
                ySum += membershipPower * dataPoints[j].y;
                membershipSum += membershipPower;
            }

            newCenters[i][0] = xSum / membershipSum;
            newCenters[i][1] = ySum / membershipSum;
        }

        // Check for convergence
        int isConverged = 1;
        for (i = 0; i < numClusterPoints; i++) {
            if (calculateDistance(clusterPoints[i].center, (Point) { newCenters[i][0], newCenters[i][1] }) > EPSILON) {
                isConverged = 0;
                break;
            }
        }

        if (isConverged) {
            break;
        }

        // Update membership matrix
        for (i = 0; i < numDataPoints; i++) {
            for (j = 0; j < numClusterPoints; j++) {
                double distance = calculateDistance(dataPoints[i], (Point) { newCenters[j][0], newCenters[j][1] });
                oldMembershipMatrix[i][j] = membershipMatrix[i][j];
                membershipMatrix[i][j] = 1.0 / (pow(distance, 2) + EPSILON);
            }
        }

        // Normalize membership matrix
        for (i = 0; i < numDataPoints; i++) {
            double membershipSum = 0.0;
            for (j = 0; j < numClusterPoints; j++) {
                membershipSum += membershipMatrix[i][j];
            }
            for (j = 0; j < numClusterPoints; j++) {
                membershipMatrix[i][j] /= membershipSum;
            }
        }

        // Update cluster centers and membership values
        for (i = 0; i < numClusterPoints; i++) {
            clusterPoints[i].center.x = newCenters[i][0];
            clusterPoints[i].center.y = newCenters[i][1];
            clusterPoints[i].membership = 0.0;
        }

        for (i = 0; i < numDataPoints; i++) {
            for (j = 0; j < numClusterPoints; j++) {
                clusterPoints[j].membership += pow(membershipMatrix[i][j], 2);
            }
        }

        iteration++;
    }
}

int main() {
    // Example usage
    Point dataPoints[MAX_DATA_POINTS] = {
        {1.0, 2.0},
        {2.0, 1.5},
        {3.0, 1.0},
        {5.0, 4.0},
        {6.0, 5.0}
    };

    ClusterPoint clusterPoints[MAX_CLUSTER_POINTS];

    fuzzyCMeans(dataPoints, 5, clusterPoints, 2);

    for (int i = 0; i < 2; i++) {
        printf("Cluster %d center: (%f, %f)\n", i + 1, clusterPoints[i].center.x, clusterPoints[i].center.y);
    }

    return 0;
}