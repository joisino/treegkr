#include <bits/stdc++.h>
#include "util.hpp"

using namespace std;

struct node{
  using np = node*;
  np p, chl, chr;
  double val, add, len, size, nb;
  node(){}
  node(double val, double add, double len, double size, double nb, np p, np chl, np chr) : val(val), add(add), len(len), size(size), nb(nb), p(p), chl(chl), chr(chr){}
  bool is_root(){
    return !p;
  }
  void rot();
  np splay();
  int pos(){
    if(p){
      if(p->chl == this) return -1;
      if(p->chr == this) return 1;
    }
    return 0;
  }
};

using np = node*;

node dummy(0, 0, 0, 0, 0, NULL, NULL, NULL);

vector<node> nodes;
int it;

inline double size(np t){ return t->size; }
inline int nb(np t){ return t->nb; }

np newnode(double val, double len, double add, np p){
  nodes[it] = node(val, add, len, len, 1, p, &dummy, &dummy);
  return &nodes[it++];
}

void push(np t){
  t->val += t->add;
  t->chl->add += t->add;
  t->chr->add += t->add;
  t->add = 0;
}

np update(np t){
  if(!t || t == &dummy) return t;
  t->size = size(t->chl) + size(t->chr) + t->len;
  t->nb = nb(t->chl) + nb(t->chr) + 1;
  return t;
}


void node::rot(){
  np q = p->p;
  int pps = p->pos();
  if(pps == -1) q->chl = this;
  if(pps == 1 ) q->chr = this;
  int ps = pos();
  if(ps == -1){
    p->chl = chr;
    chr->p = p;
    chr = p;
  } else if(ps == 1){
    p->chr = chl;
    chl->p = p;
    chl = p;
  }
  p->p = this;
  update(p);
  p = q;
  update(this);
  update(q);
}

np node::splay(){
  while(!is_root()){
    int ps = pos();
    int pps = p->pos();
    if(pps == 0){
      rot();
    } else if(ps == pps){
      p->rot();
      rot();
    } else {
      rot();
      rot();
    }
  }
  return this;
}


np le(np t){
  assert(t != &dummy);
  push(t);
  if(t->chl != &dummy) return le(t->chl);
  return t->splay();
}

np re(np t){
  assert(t != &dummy);
  push(t);
  if(t->chr != &dummy) return re(t->chr);
  return t->splay();
}

np find(np t, double k, double c){
  if(t == &dummy) return t;
  if(t->size <= k){
    t->add -= c;
    return t;
  }
  push(t);
  if(k < size(t->chl)) return find(t->chl, k, c);
  else if(k < size(t->chl) + t->len){
    double ck = k - size(t->chl);
    np res = t->splay();
    np l = newnode(t->val, ck, -c, res);
    res->len -= ck;
    l->chl = res->chl;
    res->chl->p = l;
    res->chl = l;
    res->val += c;
    res->chr->add += c;
    update(l);
    return update(res);
  }
  return find(t->chr, k - size(t->chl) - t->len, c);
}

struct TH{
  np root;
  double ll, rr;
  double ini;
  double pos;
  TH(){}
};

np insert(np &t, np p, double val, double len){
  if(t == &dummy){
    np cur = newnode(val, len, 0, p);
    t = cur;
    return cur->splay();
  }
  push(t);
  if(val < t->val){
    return insert(t->chl, t, val, len);
  } else if(val > t->val){
    return insert(t->chr, t, val, len);
  } else {
    t->len += len;
    return update(t)->splay();
  }
}


np insert(np &t, np p, np x){
  if(t == &dummy){
    x->p = p;
    x->chl = x->chr = &dummy;
    t = x;
    return update(x)->splay();
  }
  push(t);
  if(x->val < t->val){
    return insert(t->chl, t, x);
  } else if(x->val > t->val){
    return insert(t->chr, t, x);
  } else {
    t->len += x->len;
    return update(t)->splay();
  }
}

void merge(np &t, np s, double ll, double rr){
  if(s == &dummy){
    return;
  }
  push(s);
  merge(t, s->chl, ll, rr);
  merge(t, s->chr, ll, rr);
  t = insert(t, NULL, s);
}

void treegkr_dfs(Tree &T, vector<double> &a, vector<double> &creation, vector<double> &deletion, vector<TH> &dp, int v, int p){
  dp[v].ll = -creation[v];
  dp[v].rr = deletion[v];
  if(a[v] >= 0){
    dp[v].root = newnode(-creation[v], a[v], 0, NULL);
    dp[v].ini = a[v] * creation[v];
    dp[v].pos = 0;
  } else {
    dp[v].root = newnode(deletion[v], -a[v], 0, NULL);
    dp[v].ini = 0;
    dp[v].pos = a[v];
  }
  for(pair<int, double> it: T.G[v]){
    int i = it.first;
    double c = it.second;
    if(i == p){
      continue;
    }
    treegkr_dfs(T, a, creation, deletion, dp, i, v);
    dp[i].root = find(dp[i].root, -dp[i].pos, c);
    dp[i].ini -= dp[i].pos * c;
    dp[i].ll -= c;
    dp[i].rr += c;

    dp[v].ini += dp[i].ini;
    dp[v].pos += dp[i].pos;
    dp[v].ll = max(dp[v].ll, dp[i].ll);
    dp[v].rr = min(dp[v].rr, dp[i].rr);

    if(dp[v].root == &dummy){
      dp[v].root = dp[i].root;
    } else if(dp[i].root != &dummy){
      if(dp[v].root->nb < dp[i].root->nb){
        swap(dp[i].root, dp[v].root);
      }
      merge(dp[v].root, dp[i].root, dp[v].ll, dp[v].rr);
    }

    while(dp[v].root != &dummy){
      dp[v].root = le(dp[v].root);
      if(dp[v].root->val >= dp[v].ll){
        break;
      }
      dp[v].ini += dp[v].root->val * dp[v].root->len;
      dp[v].pos += dp[v].root->len;
      dp[v].root = dp[v].root->chr;
      dp[v].root->p = NULL;
    }

    if(dp[v].pos > 0){
      dp[v].root = insert(dp[v].root, NULL, dp[v].ll, dp[v].pos);
      dp[v].ini -= dp[v].ll * dp[v].pos;
      dp[v].pos = 0;
    }

    while(dp[v].root != &dummy){
      dp[v].root = re(dp[v].root);
      if(dp[v].root->val <= dp[v].rr){
        break;
      }
      dp[v].root = dp[v].root->chl;
      dp[v].root->p = NULL;
    }

    if(dp[v].pos + size(dp[v].root) < 0){
      dp[v].root = insert(dp[v].root, NULL, dp[v].rr, -(dp[v].pos + size(dp[v].root)));
    }
  }
}

double treegkr(Tree T, vector<double> creation, vector<double> deletion, vector<double> x, vector<double> y){
  int n = x.size();

  nodes.resize(4 * n + 1);
  it = 0;

  vector<TH> dp(n);

  vector<double> a(n);
  for(double i = 0; i < n; i++){
    a[i] = y[i] - x[i];
  }
  treegkr_dfs(T, a, creation, deletion, dp, 0, -1);
  
  double pos = dp[0].pos;
  double ans = dp[0].ini;

  if(dp[0].root == &dummy){
    return ans;
  }
  
  while(1){
    dp[0].root = le(dp[0].root);
    if(pos + dp[0].root->len >= 0){
      return ans - dp[0].root->val * pos;
    } else {
      pos += dp[0].root->len;
      ans += dp[0].root->val * dp[0].root->len;
    }
    dp[0].root = dp[0].root->chr;
  }
}

