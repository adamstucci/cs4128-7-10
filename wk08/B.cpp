#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>

// Was originally failing because forgot to account for there being multiple
// roads between two cities
// Thanks Raveen for the debugging help

using namespace std;
typedef long long ll;

const ll INF = 1LL << 60;

struct FlowNetwork {
    int n;
    vector<vector<ll>> adjMat, adjList;
    // level[v] stores dist from s to v
    // uptochild[v] stores first non-useless child.
    vector<int> level, uptochild;

    FlowNetwork (int _n): n(_n) {
        // adjacency matrix is zero-initialised
        adjMat.resize(n);
        for (int i = 0; i < n; i++)
            adjMat[i].resize(n);
        adjList.resize(n);
        level.resize(n);
        uptochild.resize(n);
    }

    void add_edge (int u, int v, ll c) {
        // add to any existing edge without overwriting
        adjMat[u][v] += c;        
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    void flow_edge (int u, int v, ll c) {
        adjMat[u][v] -= c;
        adjMat[v][u] += c;
    }

    // constructs the level graph and returns whether the sink is still reachable
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(s);
        level[s] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            uptochild[u] = 0; // reset uptochild
            for (int v : adjList[u])
                if (adjMat[u][v] > 0) {
                    if (level[v] != -1) // already seen
                        continue;
                    level[v] = level[u] + 1;
                    q.push(v);
                }
        }
        return level[t] != -1;
    }

    // finds an augmenting path with up to f flow.
    ll augment(int u, int t, ll f) {
        if (u == t) return f; // base case.
        // note the reference here! we increment uptochild[u], i.e. walk through u's neighbours
        // until we find a child that we can flow through
        for (int &i = uptochild[u]; i < adjList[u].size(); i++) {
            int v = adjList[u][i];
            if (adjMat[u][v] > 0) {
                // ignore edges not in the BFS tree.
                if (level[v] != level[u] + 1) continue;
                // revised flow is constrained also by this edge
                ll rf = augment(v,t,min(f, adjMat[u][v]));
                // found a child we can flow through!
                if (rf > 0) {
                    flow_edge(u,v,rf);
                    return rf;
                }
            }
        }
        level[u] = -1;
        return 0;
    }

    ll dinic(int s, int t) {
        ll res = 0;
        while (bfs(s,t))
            for (ll x = augment(s,t,INF); x; x = augment(s,t,INF))
                res += x;
        return res;
    }
};

ll dist[610][610];
int starts[610]; //how many people are in each city at the start
int unique_starts[610]; //list of the starting vertices
int num_unique; //size of this list

// can optimise by only have the starting cities in the first layer

int main() {
    
    // init dist to inf
    for (int i = 0; i < 610; i++) {
        for (int j = 0; j < 610; j++) {
            dist[i][j] = INF;
        }

        // zero out diagonal
        dist[i][i] = 0;
    }

    int v, e, n;
    ll k;
    scanf("%d %d %d %lld", &v, &e, &n, &k);

    for (int i = 1; i <= n; i++) {
        int start;
        scanf("%d", &start);

        // city not seen yet
        if (starts[start] == 0) {
            unique_starts[num_unique + 1] = start; //1-based indexing for convenience
            num_unique++;
        }

        starts[start] += 1;
    }

    // using 1-based indexing
    // convenient because super source will have index 0 in our flow network
    for (int i = 0; i < e; i++) {
        int a, b, t;
        scanf("%d %d %d", &a, &b, &t);
        dist[a][b] = min(dist[a][b],(ll)t);
        dist[b][a] = min(dist[b][a],(ll)t);
    }

    // compute all pairs shortest path
    // pretty sure int should be large enough for distances
    for (int k = 1; k <= v; k++) {
        for (int i = 1; i <= v; i++) {
            for (int j = 1; j <= v; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    // binary search over time taken
    // the question tells us an acceptting time will never be larger than 1731311
    // time 0 is possible if we don't need to move
    // i.e. always draw a self edge regardless of time

    // inclusive endpoints
    ll l = 0;
    ll r = 1731311;

    ll best = 2731311; //some number large enough to indicate failure

    // don't need to reconstruct the winning network which is convenient
    // can just update best

    while (l <= r) {
        ll m = (l + r) / 2; //there are more overflow safe ways to compute midpoint but doesn't matter here

        struct FlowNetwork f(2 + num_unique + v);

        for (int i = 1; i <= num_unique; i++) {
            // connect src to first layer
            f.add_edge(0, i, starts[unique_starts[i]]);

            // connect reachable nodes
            // includes self as distance zero
            for (int j = 1; j <= v; j++) {
                if (dist[unique_starts[i]][j] <= m) f.add_edge(i, num_unique + j, INF);
            }
        }

        // connect second layer to sink
        for (int j = 1; j <= v; j++) {
            f.add_edge(num_unique + j, 1 + num_unique + v, 1);
        }

        auto res = f.dinic(0, 1 + num_unique + v);

        if (res >= k) {
            //success try for smaller time
            r = m - 1;
            best = m;
        }
        else {
            //failure try for larger time
            l = m + 1;
        }
    }

    if (best <= 1731311) {
        printf("%lld\n", best);
    }
    else {
        printf("-1\n");
    }

}