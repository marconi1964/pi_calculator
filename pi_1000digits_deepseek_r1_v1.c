#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIGITS 1050  // 多計算幾位以避免誤差
#define MAX_TERMS 100000  // 最大迭代次數以防無限循環

typedef struct {
    char d[DIGITS];
    int sign;
} HPNum;

// 在 typedef struct HPNum 之後添加以下宣告
void hp_subtract(HPNum *result, const HPNum *a, const HPNum *b);

void hp_init(HPNum *hp, int a, int b) {
    memset(hp->d, 0, DIGITS);
    hp->sign = (a * b < 0) ? -1 : 1;
    a = abs(a);
    b = abs(b);
    int remainder = a % b;
    for (int i = 0; i < DIGITS; i++) {
        remainder *= 10;
        hp->d[i] = remainder / b;
        remainder %= b;
    }
}

// 新版本輸出 1000 位 + 四捨五入處理
void hp_print(const HPNum *hp) {
    printf("%c0.", hp->sign == 1 ? '+' : '-');
    
    // 複製數值以避免修改原數據
    char temp[DIGITS];
    memcpy(temp, hp->d, DIGITS);
    
    // 處理四捨五入
    if(temp[1000] >= 5) {  // 檢查第1001位
        for(int i=999; i>=0; i--) {
            temp[i]++;
            if(temp[i] < 10) break;
            temp[i] = 0;
        }
    }
    
    // 輸出前1000位
    for(int i=0; i<1000; i++) {
        printf("%d", temp[i]);
    }
    printf("\n");
}

void hp_add(HPNum *result, const HPNum *a, const HPNum *b) {
    if (a->sign != b->sign) {
        HPNum tmp = *b;
        tmp.sign = -tmp.sign;
        hp_subtract(result, a, &tmp);
        return;
    }

    result->sign = a->sign;
    int carry = 0;
    for (int i = DIGITS-1; i >= 0; i--) {
        int sum = a->d[i] + b->d[i] + carry;
        result->d[i] = sum % 10;
        carry = sum / 10;
    }
}

void hp_subtract(HPNum *result, const HPNum *a, const HPNum *b) {
    if (a->sign != b->sign) {
        HPNum tmp = *b;
        tmp.sign = -tmp.sign;
        hp_add(result, a, &tmp);
        return;
    }

    HPNum a_abs = *a, b_abs = *b;
    a_abs.sign = b_abs.sign = 1;

    int cmp = 0;
    for (int i = 0; i < DIGITS; i++) {
        if (a->d[i] != b->d[i]) {
            cmp = (a->d[i] > b->d[i]) ? 1 : -1;
            break;
        }
    }

    if (cmp == 0) {
        hp_init(result, 0, 1);
        return;
    }

    const HPNum *larger = cmp > 0 ? a : b;
    const HPNum *smaller = cmp > 0 ? b : a;
    
    result->sign = (cmp == 1) ? a->sign : -a->sign;
    int borrow = 0;
    for (int i = DIGITS-1; i >= 0; i--) {
        int diff = larger->d[i] - smaller->d[i] - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->d[i] = diff;
    }
}

void hp_multiply_int(HPNum *result, const HPNum *hp, int n) {
    result->sign = hp->sign * (n < 0 ? -1 : 1);
    n = abs(n);
    int carry = 0;
    for (int i = DIGITS-1; i >= 0; i--) {
        int product = hp->d[i] * n + carry;
        result->d[i] = product % 10;
        carry = product / 10;
    }
}

void hp_divide_int(HPNum *result, const HPNum *hp, int n) {
    result->sign = hp->sign * (n < 0 ? -1 : 1);
    n = abs(n);
    int remainder = 0;
    for (int i = 0; i < DIGITS; i++) {
        int temp = hp->d[i] + remainder * 10;
        result->d[i] = temp / n;
        remainder = temp % n;
    }
}

void calculate_arctan(HPNum *result, int x) {
    HPNum term, sum, tmp;
    hp_init(&sum, 0, 1);
    hp_init(&term, 1, x);

    int k = 1;
    while (k < MAX_TERMS) {
        // 加到總和
        hp_add(&tmp, &sum, &term);
        sum = tmp;

        // 更新項
        hp_multiply_int(&tmp, &term, -(2*k - 1));
        term = tmp;

        int divisor = x*x*(2*k + 1);
        hp_divide_int(&tmp, &term, divisor);
        term = tmp;

        // 新收斂檢查 (嚴格條件)
        int is_negligible = 1;
        for (int i = 0; i < DIGITS - 50; i++) {  // 檢查到更深的位數
            if (term.d[i] != 0) {
                is_negligible = 0;
                break;
            }
        }
        if (is_negligible) break;
        k++;
    }
    *result = sum;
}

void calculate_pi(HPNum *pi) {
    HPNum arctan5, arctan239, t1, t2;
    
    calculate_arctan(&arctan5, 5);
    calculate_arctan(&arctan239, 239);
    
    // 16*arctan5
    hp_multiply_int(&t1, &arctan5, 16);
    
    // 4*arctan239
    hp_multiply_int(&t2, &arctan239, 4);
    
    // pi = 16*arctan5 - 4*arctan239
    hp_subtract(pi, &t1, &t2);
}

int main() {
    HPNum pi;
    calculate_pi(&pi);
    printf("Pi calculated to 100 digits:\n");
    hp_print(&pi);
    return 0;
}
