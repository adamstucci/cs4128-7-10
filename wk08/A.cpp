#include <cstdio>
#include <vector>
#include <queue>

using namespace std;
typedef long long ll;

ll INF = 1LL << 60;

ll safe_delta(ll a, ll b) {
    return a < b ? b - a : a - b;
}

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

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    vector<int> init_armies;
    ll in_units = 0;
    for (int i = 0; i < n; i++) {
        int a;
        scanf("%d", &a);
        in_units += a;
        init_armies.push_back(a);
    }

    vector<int> expected_armies;
    ll out_units = 0;
    for (int i = 0; i < n; i++) {
        int b;
        scanf("%d", &b);
        out_units += b;
        expected_armies.push_back(b);
    }

    if (in_units != out_units) {
        printf("NO\n");
        return 0;
    }

    // Flow network construction
    // 2n + 2 vertices

    struct FlowNetwork f(2*n + 2);

    //connect source to first layer
    //connect last layer to sink
    //connect vertex to itself
    for (int i = 0; i < n; i++) {
        f.add_edge(0, i+1, init_armies[i]);
        f.add_edge(i+1+n, 2*n+1, expected_armies[i]);
        f.add_edge(i+1, i+1+n, INF);
    }

    //connect middle layers
    for (int i = 0; i < m; i++) {
        int p, q;
        scanf("%d %d", &p, &q);
        f.add_edge(p, q+n, INF);
        f.add_edge(q, p+n, INF);
    }

    ll flow = f.dinic(0, 2*n+1);

    if (flow == out_units) {
        printf("YES\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // back edges are all synthetic
                // they hold exactly the flow sent through the forward edge
                printf("%lld", f.adjMat[j+1+n][i+1]);
                if (j < n-1) printf(" ");
            }
            printf("\n");
        }
    }
    else {
        printf("NO\n");
    }

}