struct Complex{ float real, image;};
int main(){
struct Complex c;
float f;
c.real = 5.1;
c.image = 3.2;
f = c.real + c.image;
write(f);
return 0;
}
