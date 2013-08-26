

D2P *d2p_open(){
}

d2p_close(D2P *p){
}


static uint32_t crypttable[]={
};




static uint32_t hashval(const char *str, uint32_t seed)
{
    char *p;
    char ch;
    uint32_t seed1,seed2;

    p=str;
    seed1=0x7FED7FED;
    seed2=0xEEEEEEEE;

    while(*p!=0){
        ch = toupper(*p++);
        seed1 = crypttable[(seed<<8)+ch]^(seed1+seed2);
        seed2 = ch+seed1+seed2+(seed2<<5)+3;
    }
    return seed1;
}
