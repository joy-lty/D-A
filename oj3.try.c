#include<stdio.h>
#include<string.h>

#define MAXN 100010
#define MAXM 4010
#define MAXK 240010
#define INF 0x3f3f3f3f

typedef struct {
    int to;
    unsigned char price;
} Edge;//边的结构体存储图
Edge edges[MAXK];
int edge_count = 0;
int head[MAXM];//每个节点的第一条边
int next[MAXK];//边的后继边

void add_edge(int u, int v, int w) {
    Edge e;
    e.to = v;
    e.price = w;
    edges[edge_count] = e;
    next[edge_count] = head[u];
    head[u] = edge_count;
    edge_count++;
}
//最小堆
typedef struct {
    int node;
    int dist;
} HeapNode;
HeapNode heap[MAXM * 5];
int heap_size = 0;
void up(int i) {
    while (i > 1) {
        int p = i >> 1;
        if (heap[p].dist <= heap[i].dist) break;
        HeapNode tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}//向上堆化
void down(int i) {
    while (1) {
        int l = i << 1, r = l + 1, min_i = i;
        if (l <= heap_size && heap[l].dist < heap[min_i].dist) min_i = l;
        if (r <= heap_size && heap[r].dist < heap[min_i].dist) min_i = r;
        if (min_i == i) break;
        HeapNode tmp = heap[min_i];
        heap[min_i] = heap[i];
        heap[i] = tmp;
        i = min_i;
    }
}//向下堆化
void push(int node, int dist) {
    heap_size++;
    heap[heap_size].node = node;
    heap[heap_size].dist = dist;
    up(heap_size);
}
HeapNode pop() {
    HeapNode top = heap[1];
    heap[1] = heap[heap_size--];
    down(1);
    return top;
}
// Dijkstra 算法
int dist[MAXM];
void dijkstra(int start, int M) {
    static int visited[MAXM];
    for (int i = 1; i <= M; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    heap_size = 0;
    dist[start] = 0;
    push(start, 0);
    while (heap_size > 0) {
        HeapNode h = pop();
        int u = h.node;
        if (visited[u]) continue;
        visited[u] = 1;
        for (int i = head[u]; i != -1; i = next[i]) {
            int v = edges[i].to;
            int w = edges[i].price;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                push(v, dist[v]);
            }
        }
    }
}

int main() {
    int N,M,K;
    scanf("%d %d %d", &N, &M, &K);
    for (int i = 1; i <= M; i++) head[i] = -1;
    for(int i=0;i<K;i++){
        int from,to,price;
        scanf("%d %d %d",&from,&to,&price);
        add_edge(to,from,price);
    }//存储反向图
    int city[MAXM]={0};
    for (int i = 1; i <= N; i++) {
        int t;
        scanf("%d", &t);
        city[t]++;
    }
    dijkstra(1, M);
    int dist_toC[MAXM];
    memcpy(dist_toC, dist, sizeof(dist));

    int best_city = -1;
    long long best_cost = INF;
    for (int i = 2; i <= M; i++) {//主循环
        if (head[i]==-1&&city[i]==0) continue;//i城不可及且无参会人员
        dijkstra(i, M);
        long long total_cost = 0;
        for (int j = 1; j <= M; j++) {
            if (city[j] == 0) continue;//无参会人员直接跳
            total_cost += (long long)city[j] * (dist[j] < dist_toC[j] ? dist[j] : dist_toC[j]);
            if (total_cost >= best_cost) break;
        }
        if (total_cost < best_cost) {
            best_city = i;
            best_cost = total_cost;
        }
    }
    printf("%d\n%lld\n", best_city, best_cost);
    return 0;
}