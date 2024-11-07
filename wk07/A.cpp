#include <cstdio>
#include <algorithm>

using namespace std;
typedef long long ll;


ll D[310][310];

int main() {

    int n;
    scanf("%d", &n);

    //read in distances matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%lld", &D[i][j]);
        }
    }

    int k;
    scanf("%d", &k);

    for (int i = 0; i < k; i++) {
        int a;
        int b;
        int c;
        scanf("%d %d %d", &a, &b, &c);

        //1-based to 0-based indexing
        a--; b--;

        //update min distances
        for (int u = 0; u < n; u++) {
            for (int v = u+1; v < n; v++) {
                D[u][v] = min({D[u][v], D[u][a] + c + D[b][v], D[u][b] + c + D[a][v]});
                D[v][u] = D[u][v];
            }
        }

        //sum unordered all pairs shortest paths
        ll sum = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                sum += D[i][j];
            }
        }

        printf("%lld", sum);
        if (i < k-1) printf(" ");
    }

    printf("\n");
    
}