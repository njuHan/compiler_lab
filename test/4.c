int main(){
int i = 5, j = 2, k=3, min=i, max=i;
if(j > max){
max = j;
if(k > max) max = k; }
else if(j < min){ min = j;
if(k < min) min = k; }
write(max); write(min);
}
