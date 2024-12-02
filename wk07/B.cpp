#include <cstdio>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
typedef long long ll;


int n; //global for convenience

const int max_edges = 200010; //suitably large number upper bound for distance

// prefix sums and min_price need to be long long but everything else can be int?
// weights are made long long for convenience so don't have to do cast with arithmetic operations

// global so bfs function doesn't have to move around to many arguments
// vector for easier construction and teardown
vector<vector<int>> adj_list;

// graph connected, so will visit every vertex
// shortest hops...i.e. BFS
// should do copy elision optimization?
// compute shortest hops from s to all other vertices
vector<int> shortest_hops(int s) {
    vector<int> hops(n, max_edges);

    //(node, hops)
    queue<pair<int, int>> q;
    q.push(make_pair(s, 0));

    while (!q.empty()) {
        pair<int, int> elem = q.front();
        q.pop();
        int u = elem.first;
        int h = elem.second;

        // if already visited
        if (hops[u] < max_edges) continue;

        hops[u] = h;

        for (int v : adj_list[u]) q.push(make_pair(v, h+1));
    }

    return hops;
}

int main() {
    int t;
    scanf("%d", &t);

    for (int i = 0; i < t; i++) {
        int m, a, b, c;
        scanf("%d %d %d %d %d", &n, &m, &a, &b, &c);
        
        // 1-based to 0-based
        a--; b--; c--;

        vector<ll> weights;
        for (int i = 0; i < m; i++) {
            ll w;
            scanf("%lld", &w);
            weights.push_back(w);
        }

        sort(weights.begin(), weights.end());

        vector<ll> prefix;
        
        // convenient sum of zero edges so don't have to decrement later
        prefix.push_back(0); 

        for (int i = 0; i < weights.size(); i++) {
            prefix.push_back(weights[i] + prefix[i]);
        }

        adj_list = vector<vector<int>>(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            scanf("%d %d", &u, &v);
            u--; v--;
            adj_list[u].push_back(v);
            adj_list[v].push_back(u);
        }

        ll min_price = 1e18; //large initial value

        // don't need to redo a BFS for every intermediary vertex
        // compute once and query table based on intermediary vertex
        auto hops_from_a = shortest_hops(a);
        auto hops_from_b = shortest_hops(b);
        auto hops_from_c = shortest_hops(c);

        for (int z = 0; z < n; z++) {
            
            // order doesn't matter here because graph is undirected
            int edges1 = hops_from_a[z];    //dist a to z
            int edges2 = hops_from_b[z];    //dist z to b...gets travelled agin in b to z
            int edges3 = hops_from_c[z];    //dist z to c

            //bounds check so don't overflow vector
            if (edges1 + edges2 + edges3 > m) continue;

            min_price = min(min_price, prefix[edges2] + prefix[edges1 + edges2 + edges3]);

        }

        printf("%lld\n", min_price);

    }
}