#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;
using namespace chrono;

// Define some convenient macros
typedef long long ll;
#define vec vector <int>
#define mus microseconds
#define setprec cout << fixed << showpoint << setprecision(20);
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

int n, bign;
vector <vec> dist, dp, originalDist;
vec c;

int tsp(int mask, int last) {
    if (mask == (1 << bign) - 1) return dist[last][0];
    if (dp[mask][last] != -1) return dp[mask][last];

    int res = inf;
    for (int i = 0; i < bign; i++) {
        if (mask & (1 << i)) continue;
        res = min(res, dist[last][i] + tsp(mask | (1 << i), i));
    }
    return dp[mask][last] = res;
}

signed main()
{
    // Taking the input 
    cin >> n;
    c.resize(n);
    FOR (i, 0, n - 1) 
    {
        cin >> c[i];
        bign += c[i];
    }

    originalDist.assign(n, vec(n));
    FOR (i, 0, n - 1)
        FOR (j, 0, n - 1)
            cin >> originalDist[i][j];

    // Actual code starts
    {
        Timer timer;
        dist.assign(bign, vec(bign));
        vec mapper;
        FOR (i, 0, n - 1)
            FOR (j, 1, c[i])
                mapper.push_back(i);

        FOR (i, 0, bign - 1)
            FOR (j, 0, bign - 1)
                dist[i][j] = originalDist[mapper[i]][mapper[j]];
        
        dp.assign(1 << bign, vec(bign, inf));
        dp[1][0] = 0;
        FOR (mask, 2, (1 << bign) - 1)
        {
            if (mask % 2 == 0) continue;
            FOR (last, 1, bign - 1)
            {
                if (!(mask & (1 << last))) continue;
                FOR (sl, 0, bign - 1)
                {
                    if (!(mask & (1 << sl))) continue;
                    if (last == sl) continue;
                    dp[mask][last] = min(dp[mask][last],
                    dp[mask - (1 << last)][sl] + dist[sl][last]);
                }
            }
        }
        
        int ans = inf;
        FOR (i, 1, bign - 1)
            ans = min(ans, dp[(1 << bign) - 1][i] + dist[i][1]);
        
        cout << ans << endl;
    }
}
