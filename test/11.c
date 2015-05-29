struct Complex{ float re, im;};
float sum(struct Complex x){ return x.re + x.im;}
int main(){
struct Complex c;
float f;
c.re = 5.1;
c.im = 3.2;
f = sum(c);
write(f);
}
