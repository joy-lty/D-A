#include <stdio.h>

int main() {
    int M, N;
    scanf("%d", &M);
    scanf("%d", &N);
    float p;
    scanf("%f", &p);
    static int f[20][20];
    float t;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%f", &t);
            f[i][j] = (t > p);
        }
    }
    static int h[20];
    static int stack[20], l[20], r[20];
    int Max = 0, up = 0, down = 0, left = 0, right = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            h[j] = f[i][j] ? h[j] + 1 : 0;
        }
        int top = -1;
        for (int j = 0; j < N; j++) {
            while (top >= 0 && h[stack[top]] >= h[j]) top--;
            l[j] = (top >= 0 ? stack[top] + 1 : 0);
            stack[++top] = j;
        }
        top = -1;
        for (int j = N - 1; j >= 0; j--) {
            while (top >= 0 && h[stack[top]] >= h[j]) top--;
            r[j] = (top >= 0 ? stack[top] - 1 : N - 1);
            stack[++top] = j;
        }
        for (int j = 0; j < N; j++) {
            if (h[j] == 0) continue;
            int width = r[j] - l[j] + 1;
            int perim = 2 * (h[j] + width);
            if (perim > Max) {
                Max = perim;
                up = i - h[j] + 1;
                down = i;
                left = l[j];
                right = r[j];
            }
        }
    }
    printf("%d %d\n%d %d\n%d\n", up, down, left, right, Max);
    return 0;
}