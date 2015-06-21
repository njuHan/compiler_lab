int main(){
int a[5], i = 0, j = 0, n = 5;
while(i < n){ a[i] = 10 - 2*i; i = i + 1;} 
i = 0;
while (i < n){ j = 0;
while (j < i){
if (a[i] < a[j]){ int t = a[i]; a[i] = a[j]; a[j] = t;}
j = j + 1; } i = i + 1; }
i = 0; while (i < n){ write(a[i]); i = i + 1; } 
return 0;
}
