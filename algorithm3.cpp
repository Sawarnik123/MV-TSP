#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;
using namespace chrono;

// Define some convenient macros
#define sz(v) (long long)(v.size())
#define pback push_back
#define vec vector <int>
#define mus microseconds
#define FOR(j, a, b) for (int j = a; j <= b; j++)
#define FORD(j, a, b) for (int j = a; j >= b; j--)
const int inf = 1e17;
mt19937 rng(steady_clock::now().time_since_epoch().count());

class Timer {
public:
    Timer() {
        m_pointStart = high_resolution_clock::now();
    }
    ~Timer() {
        Stopper();
    }
    void Stopper() {
        auto pointEnd = high_resolution_clock::now();

        auto startup = time_point_cast<mus>
        (m_pointStart).time_since_epoch().count();
        auto end = time_point_cast<mus>
        (pointEnd).time_since_epoch().count();

        auto dura = end - startup;
        long double ms = dura * 0.001;
        cout << dura << "us\n";
    }

private:
    time_point<high_resolution_clock> m_pointStart;
};

int n;
vector <vec> dist;
vec c;

map <vec, int> dp;

int solve (vec& seq, int sum)
{
    if (!sum) return 0;
    if (!seq[0]) return inf;
    if (dp[seq]) return dp[seq];
    
    int ans = inf;
    
    FOR (leaf, 0, n - 1)
        if (!seq[leaf])
        {
            seq[leaf] = inf;
            FOR (parent, 0, n - 1)
            {
                if (seq[parent] == inf || !seq[parent]) continue;
                seq[parent]--;
                ans = min(ans, solve(seq, sum - 1)) + dist[parent][leaf];
                seq[parent]++;
            }
            seq[leaf] = 0;
            break;
        }
        
    return dp[seq] = ans;
}

struct edge
{
    int x, y, cap, cost;
};

int sink;
vector <edge> e;
void createEdge (int x, int y, int cap, int cost)
{
    e.pback({x, y, cap, cost});
    e.pback({y, x, 0, -cost});
}
 
void createGraph (vec& seq)
{
    //source to A
    FOR (i, 0, n - 1) createEdge(0, i + 1, c[i] - seq[i], 0);
 
    //A to B
    FOR (i, 0, n - 1)
        FOR (j, 0, n - 1)
            createEdge(i + 1, j + n + 1, inf, dist[i][j]);
 
    //B to sink
    FOR (i, 0, n - 1) createEdge(i + n + 1, sink, c[i] - bool(i), 0);
}
 
void shortest_path (vec& d, vec& p)
{
    d.assign(sink + 1, inf);
    p.assign(sink + 1, -1);
 
    d[0] = 0;
    FOR (i, 0, sink)
    {
        bool done = 0;
        FOR (j, 0, sz(e) - 1)
        {
            auto q = e[j];
            if (d[q.x] < inf && q.cap > 0 && d[q.y] > d[q.x] + q.cost)
            {
                d[q.y] = d[q.x] + q.cost;
                p[q.y] = j;
                done = 1;
            }
        }
 
        if (!done) break; 
    }
}
 
int max_flow ()
{
    vec d, p;
    int cost = 0;
 
    while (true)
    {
        shortest_path(d, p);
        if (d[sink] >= inf) break;
 
        //retrieve path
        int mn = inf, cur = sink;
        while (cur)
        {
            mn = min(mn, e[p[cur]].cap);
            cur = e[p[cur]].x;
        }
 
        //augment path
        cur = sink;
        cost += d[sink]*mn;
 
        while (cur)
        {
            e[p[cur]].cap -= mn;
            e[p[cur] ^ 1].cap += mn;
            cur = e[p[cur]].x;
        }
    }
 
    return cost;
}

int ans = inf;
void compute (vec& seq)
{
    e.clear();
    createGraph(seq);
    ans = min(ans, solve(seq, n - 1) + max_flow());
}

void iterateOverAllSequences (vec& seq, int currentIndex, int sum, bool isRoot)
{
    if (currentIndex == n - 1)
    {
        seq[n - 1] = sum;
        if (sum > c[n - 1]) return;

        compute(seq);
        return;
    }

    FOR (i, isRoot, min(sum, c[currentIndex])) 
    {
        seq[currentIndex] = i;
        iterateOverAllSequences(seq, currentIndex + 1, sum - i, false);
    }
}

signed main()
{
    // Taking input
    cin >> n;
    sink = 2*n + 1;
    c.resize(n);
    FOR (i, 0, n - 1) cin >> c[i];

    dist.assign(n, vec(n));
    FOR (i, 0, n - 1)
        FOR (j, 0, n - 1)
            cin >> dist[i][j];

    // Executing the algorithm
    {
        Timer timer;
        vec seq(n);
        // Iterate over all (out)degree sequences
        iterateOverAllSequences(seq, 0, n - 1, true);

        cout << ans << endl;
    }
}
