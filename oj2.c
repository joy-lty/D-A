#include <stdio.h>
#include <stdlib.h>

#define MAXN 1024
#define INF 0x80000000

int arr[MAXN];  
int n;          
int ans = INF;

int dfs(int index) {
    if (index >= n || arr[index] == -99) return 0;
    
    int left = dfs(2 * index + 1);
    int right = dfs(2 * index + 2);
    
    if (left < 0) left = 0;
    if (right < 0) right = 0;
    
    if (arr[index] + left + right > ans) ans = arr[index] + left + right;
    
    return arr[index] + (left > right ? left : right);
}

int main() {
    int h;
    scanf("%d", &h);
    
    n = (1 << h) - 1;  
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    dfs(0);
    printf("%d\n", ans);
    
    return 0;
}