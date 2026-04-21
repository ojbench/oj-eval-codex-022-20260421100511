#include <bits/stdc++.h>
using namespace std;

static const int MOD = 998244353; 
static const int MUL = 233;

extern int query(int x,int y,int z);

namespace Guess022 {
    vector<long long> A;

    // Find a pair (p,q) and an index m0 that lies between them such that S=query(p,q,m0)
    bool init_extremes_pair(int n, int &p, int &q, int &m0, long long &S){
        int K = min(n, 12);
        vector<int> pool(K); iota(pool.begin(), pool.end(), 1);
        while(true){
            for(int ii=0; ii<K; ++ii){
                for(int jj=ii+1; jj<K; ++jj){
                    unordered_map<long long, int> cnt;
                    unordered_map<long long, int> anyk;
                    for(int tt=0; tt<K; ++tt){
                        if(tt==ii || tt==jj) continue;
                        long long val = query(pool[ii], pool[jj], pool[tt]);
                        if(!cnt.count(val)) anyk[val] = pool[tt];
                        cnt[val]++;
                        if(cnt[val] >= 2){
                            p = pool[ii]; q = pool[jj]; m0 = anyk[val]; S = val; return true;
                        }
                    }
                }
            }
            if(K == n) break;
            int newK = min(n, K*2);
            pool.resize(newK);
            for(int t=K;t<newK;t++) pool[t] = t+1;
            K = newK;
        }
        return false;
    }

    int solve(int n, int Taskid){
        A.assign(n+1, 0);
        int p=1,q=2,m0=3; long long S = 0;
        if(!init_extremes_pair(n, p, q, m0, S)){
            // Fallback: try the first three
            p = 1; q = 2; m0 = 3; S = query(p,q,m0);
        }

        // Refine to true extremes indices with m0 between
        for(int i=1;i<=n;i++){
            if(i==p || i==q || i==m0) continue;
            long long si = query(p,q,i);
            if(si < S){
                p = i; S = query(p,q,m0);
            }else if(si > S){
                q = i; S = query(p,q,m0);
            }
        }

        // Collect three inner indices a,b,c
        vector<int> inner; inner.reserve(3);
        if(m0!=p && m0!=q){
            long long sm0 = query(p,q,m0);
            if(sm0 == S) inner.push_back(m0);
        }
        for(int i=1;i<=n && (int)inner.size()<3;i++){
            if(i==p || i==q) continue;
            if(i==m0) continue;
            long long si = query(p,q,i);
            if(si == S) inner.push_back(i);
        }
        for(int i=1;i<=n && (int)inner.size()<3;i++){
            if(i==p || i==q) continue;
            if(find(inner.begin(), inner.end(), i)!=inner.end()) continue;
            long long si = query(p,q,i);
            if(si == S) inner.push_back(i);
        }
        if((int)inner.size() < 3){
            for(int i=1;i<=n && (int)inner.size()<3;i++){
                if(i==p || i==q) continue;
                if(find(inner.begin(), inner.end(), i)==inner.end()) inner.push_back(i);
            }
        }
        int a = inner[0], b = inner[1], c = inner[2];

        // Pair sums among a,b,c: A[u]+A[v] = query(p,u,v)+query(q,u,v)-S
        long long sab = (long long)query(p,a,b) + (long long)query(q,a,b) - S;
        long long sac = (long long)query(p,a,c) + (long long)query(q,a,c) - S;
        long long sbc = (long long)query(p,b,c) + (long long)query(q,b,c) - S;

        long long Aa = (sab + sac - sbc) / 2;
        long long Ab = sab - Aa;
        long long Ac = sac - Aa;
        A[a] = Aa; A[b] = Ab; A[c] = Ac;

        // Compute extremes numeric values using a,b as helpers
        long long q_m_ab = query(p, a, b);
        long long q_M_ab = query(q, a, b);
        long long Am = q_m_ab - max(Aa, Ab);
        long long AM = q_M_ab - min(Aa, Ab);
        A[p] = Am; A[q] = AM;

        // Compute the rest using anchor a
        for(int i=1;i<=n;i++){
            if(i==p || i==q || i==a || i==b || i==c) continue;
            long long s_ia = (long long)query(p,i,a) + (long long)query(q,i,a) - S; // A[i]+A[a]
            long long Ai = s_ia - Aa;
            A[i] = Ai;
        }

        // Compute hash value
        long long ret = 0;
        for(int i=n;i>=1;i--){
            long long v = (A[i]%MOD + MOD)%MOD;
            ret = (ret + v) % MOD;
            ret = (ret * MUL) % MOD;
        }
        return (int)ret;
    }
}

int guess(int n, int Taskid){
    return Guess022::solve(n, Taskid);
}
