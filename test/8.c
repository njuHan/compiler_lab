int gcd(int a, int b){
if(b==0) return a;
else return gcd(b, a - b*(a/b) ); //a%b
}
int main(){
int i = 2013, j = 66;
write( gcd(i, j) );
}
