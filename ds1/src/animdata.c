#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "misc.h"
#include "animdata.h"
 

// ==========================================================================
UBYTE animdata_hash_value(char * name)
{

    // 这个函数输入的name和输出的hash值就是这个.. it's a very simple hash function {{{


    //   block   0:    animdata_name = L1OPHTH,          hash =   0,    nb_records =    54
    //   block   1:    animdata_name = L2OPHTH,          hash =   1,    nb_records =    57
    //   block   2:    animdata_name = F9OPHTH,          hash =   2,    nb_records =    53
    //   block   3:    animdata_name = Q1ONHTH,          hash =   3,    nb_records =    42
    //   block   4:    animdata_name = S0ONHTH,          hash =   4,    nb_records =    41
    //   block   5:    animdata_name = S1ONHTH,          hash =   5,    nb_records =    51
    //   block   6:    animdata_name = S2ONHTH,          hash =   6,    nb_records =    54
    //   block   7:    animdata_name = U1ONHTH,          hash =   7,    nb_records =    40
    //   block   8:    animdata_name = U2ONHTH,          hash =   8,    nb_records =    47
    //   block   9:    animdata_name = W1ONHTH,          hash =   9,    nb_records =    62
    //   block  10:    animdata_name = W2ONHTH,          hash =  10,    nb_records =    67
    //   block  11:    animdata_name = Y1ONHTH,          hash =  11,    nb_records =    63
    //   block  12:    animdata_name = Z1ONHTH,          hash =  12,    nb_records =    60
    //   block  13:    animdata_name = Z2ONHTH,          hash =  13,    nb_records =    64
    //   block  14:    animdata_name = Z3ONHTH,          hash =  14,    nb_records =    60
    //   block  15:    animdata_name = Z4ONHTH,          hash =  15,    nb_records =    60
    //   block  16:    animdata_name = Z5ONHTH,          hash =  16,    nb_records =    59
    //   block  17:    animdata_name = Z4OPHTH,          hash =  17,    nb_records =    62
    //   block  18:    animdata_name = Z5OPHTH,          hash =  18,    nb_records =    58
    //   block  19:    animdata_name = Z2NUHTH,          hash =  19,    nb_records =    57
    //   block  20:    animdata_name = Z3NUHTH,          hash =  20,    nb_records =    48
    //   block  21:    animdata_name = Z4NUHTH,          hash =  21,    nb_records =    51
    //   block  22:    animdata_name = Z5NUHTH,          hash =  22,    nb_records =    45
    //   block  23:    animdata_name = Y7NUHTH,          hash =  23,    nb_records =    38
    //   block  24:    animdata_name = Y8NUHTH,          hash =  24,    nb_records =    34
    //   block  25:    animdata_name = Y9NUHTH,          hash =  25,    nb_records =    40
    //   block  26:    animdata_name = UBOPHTH,          hash =  26,    nb_records =    45
    //   block  27:    animdata_name = YAONHTH,          hash =  27,    nb_records =    35
    //   block  28:    animdata_name = ZAONHTH,          hash =  28,    nb_records =    37
    //   block  29:    animdata_name = YCONHTH,          hash =  29,    nb_records =    54
    //   block  30:    animdata_name = ZCONHTH,          hash =  30,    nb_records =    53
    //   block  31:    animdata_name = ZDONHTH,          hash =  31,    nb_records =    41
    //   block  32:    animdata_name = ZEONHTH,          hash =  32,    nb_records =    50
    //   block  33:    animdata_name = ZDOPHTH,          hash =  33,    nb_records =    43
    //   block  34:    animdata_name = ZEOPHTH,          hash =  34,    nb_records =    47
    //   block  35:    animdata_name = ZHONHTH,          hash =  35,    nb_records =    45
    //   block  36:    animdata_name = ZCNUHTH,          hash =  36,    nb_records =    41
    //   block  37:    animdata_name = ZHOPHTH,          hash =  37,    nb_records =    41
    //   block  38:    animdata_name = ZENUHTH,          hash =  38,    nb_records =    36
    //   block  39:    animdata_name = ZLONHTH,          hash =  39,    nb_records =    36
    //   block  40:    animdata_name = YNONHTH,          hash =  40,    nb_records =    31
    //   block  41:    animdata_name = ZLOPHTH,          hash =  41,    nb_records =    38
    //   block  42:    animdata_name = YPONHTH,          hash =  42,    nb_records =    29
    //   block  43:    animdata_name = YQONHTH,          hash =  43,    nb_records =    19
    //   block  44:    animdata_name = ZQONHTH,          hash =  44,    nb_records =    20
    //   block  45:    animdata_name = ZRONHTH,          hash =  45,    nb_records =    23
    //   block  46:    animdata_name = ZSONHTH,          hash =  46,    nb_records =    26
    //   block  47:    animdata_name = ZROPHTH,          hash =  47,    nb_records =    16
    //   block  48:    animdata_name = ZSOPHTH,          hash =  48,    nb_records =    17
    //   block  49:    animdata_name = ZVONHTH,          hash =  49,    nb_records =    18
    //   block  50:    animdata_name = ZWONHTH,          hash =  50,    nb_records =    17
    //   block  51:    animdata_name = ZVOPHTH,          hash =  51,    nb_records =    12
    //   block  52:    animdata_name = ZYONHTH,          hash =  52,    nb_records =    15
    //   block  53:    animdata_name = YYOPHTH,          hash =  53,    nb_records =    11
    //   block  54:    animdata_name = ZYOPHTH,          hash =  54,    nb_records =    10
    //   block  55:    animdata_name = ZVNUHTH,          hash =  55,    nb_records =     5
    //   block  56:    animdata_name = ZWNUHTH,          hash =  56,    nb_records =     6
    //   block  57:    animdata_name = YYNUHTH,          hash =  57,    nb_records =     3
    //   block  58:    animdata_name = ZYNUHTH,          hash =  58,    nb_records =     4
    //   block  59:    animdata_name = ZZWLHTH,          hash =  59,    nb_records =     2
    //   block  60:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  61:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  62:    animdata_name = SOTWXBW,          hash =  62,    nb_records =     1
    //   block  63:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  64:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  65:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  66:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  67:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  68:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  69:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  70:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  71:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  72:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  73:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  74:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  75:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  76:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  77:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  78:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  79:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  80:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  81:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  82:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  83:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  84:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  85:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  86:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  87:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  88:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  89:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  90:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  91:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  92:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  93:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  94:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  95:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  96:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  97:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  98:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block  99:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 100:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 101:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 102:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 103:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 104:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 105:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 106:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 107:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 108:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 109:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 110:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 111:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 112:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 113:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 114:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 115:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 116:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 117:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 118:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 119:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 120:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 121:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 122:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 123:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 124:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 125:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 126:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 127:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 128:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 129:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 130:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 131:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 132:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 133:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 134:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 135:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 136:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 137:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 138:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 139:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 140:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 141:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 142:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 143:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 144:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 145:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 146:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 147:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 148:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 149:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 150:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 151:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 152:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 153:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 154:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 155:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 156:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 157:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 158:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 159:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 160:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 161:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 162:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 163:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 164:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 165:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 166:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 167:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 168:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 169:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 170:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 171:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 172:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 173:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 174:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 175:    animdata_name = 0AA11HS,          hash = 175,    nb_records =     1
    //   block 176:    animdata_name = 0AA21HS,          hash = 176,    nb_records =     2
    //   block 177:    animdata_name = 0AA22HS,          hash = 177,    nb_records =     1
    //   block 178:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 179:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 180:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 181:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 182:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 183:    animdata_name = 10A1HTH,          hash = 183,    nb_records =     1
    //   block 184:    animdata_name = 10A2HTH,          hash = 184,    nb_records =     1
    //   block 185:    animdata_name = 21A1HTH,          hash = 185,    nb_records =     1
    //   block 186:    animdata_name = 40A1HTH,          hash = 186,    nb_records =     3
    //   block 187:    animdata_name = 40A2HTH,          hash = 187,    nb_records =     3
    //   block 188:    animdata_name = 15A1HTH,          hash = 188,    nb_records =     2
    //   block 189:    animdata_name = 43A1HTH,          hash = 189,    nb_records =     2
    //   block 190:    animdata_name = 44A1HTH,          hash = 190,    nb_records =     2
    //   block 191:    animdata_name = 45A1HTH,          hash = 191,    nb_records =     1
    //   block 192:    animdata_name = 64A1HTH,          hash = 192,    nb_records =     3
    //   block 193:    animdata_name = 65A1HTH,          hash = 193,    nb_records =     2
    //   block 194:    animdata_name = BAA21HS,          hash = 194,    nb_records =     3
    //   block 195:    animdata_name = BAA22HS,          hash = 195,    nb_records =     4
    //   block 196:    animdata_name = BAA22HT,          hash = 196,    nb_records =     3
    //   block 197:    animdata_name = 0ADD1HS,          hash = 197,    nb_records =     2
    //   block 198:    animdata_name = 0ADD2HS,          hash = 198,    nb_records =     1
    //   block 199:    animdata_name =        ,          hash =   0,    nb_records =     0
    //   block 200:    animdata_name = 0BA1HTH,          hash = 200,    nb_records =     2
    //   block 201:    animdata_name = FEA11HS,          hash = 201,    nb_records =     7
    //   block 202:    animdata_name = 20S1HTH,          hash = 202,    nb_records =     8
    //   block 203:    animdata_name = 10S3HTH,          hash = 203,    nb_records =     5
    //   block 204:    animdata_name = F0A1HTH,          hash = 204,    nb_records =     9
    //   block 205:    animdata_name = F0A2HTH,          hash = 205,    nb_records =    10
    //   block 206:    animdata_name = 60S1HTH,          hash = 206,    nb_records =    11
    //   block 207:    animdata_name = 61S1HTH,          hash = 207,    nb_records =    13
    //   block 208:    animdata_name = 62S1HTH,          hash = 208,    nb_records =    14
    //   block 209:    animdata_name = 62S2HTH,          hash = 209,    nb_records =    10
    //   block 210:    animdata_name = 15DDHTH,          hash = 210,    nb_records =     7
    //   block 211:    animdata_name = GMA12HS,          hash = 211,    nb_records =     7
    //   block 212:    animdata_name = GMA22HS,          hash = 212,    nb_records =     9
    //   block 213:    animdata_name = L3A1HTH,          hash = 213,    nb_records =     8
    //   block 214:    animdata_name = L3A2HTH,          hash = 214,    nb_records =     7
    //   block 215:    animdata_name = M4A1HTH,          hash = 215,    nb_records =     6
    //   block 216:    animdata_name = M5A1HTH,          hash = 216,    nb_records =     7
    //   block 217:    animdata_name = M6A1HTH,          hash = 217,    nb_records =     7
    //   block 218:    animdata_name = K9A1HTH,          hash = 218,    nb_records =     5
    //   block 219:    animdata_name = K9A2HTH,          hash = 219,    nb_records =    10
    //   block 220:    animdata_name = D0S1HTH,          hash = 220,    nb_records =    12
    //   block 221:    animdata_name = D1S1HTH,          hash = 221,    nb_records =    20
    //   block 222:    animdata_name = E1S1HTH,          hash = 222,    nb_records =    21
    //   block 223:    animdata_name = E2S1HTH,          hash = 223,    nb_records =    19
    //   block 224:    animdata_name = D4S1HTH,          hash = 224,    nb_records =    21
    //   block 225:    animdata_name = D5S1HTH,          hash = 225,    nb_records =    22
    //   block 226:    animdata_name = D6S1HTH,          hash = 226,    nb_records =    20
    //   block 227:    animdata_name = D7S1HTH,          hash = 227,    nb_records =    17
    //   block 228:    animdata_name = D8S1HTH,          hash = 228,    nb_records =    15
    //   block 229:    animdata_name = D9S1HTH,          hash = 229,    nb_records =    19
    //   block 230:    animdata_name = D9S2HTH,          hash = 230,    nb_records =    26
    //   block 231:    animdata_name = D9S3HTH,          hash = 231,    nb_records =    19
    //   block 232:    animdata_name = D9S4HTH,          hash = 232,    nb_records =    16
    //   block 233:    animdata_name = 62ONHTH,          hash = 233,    nb_records =    17
    //   block 234:    animdata_name = 70OPHTH,          hash = 234,    nb_records =    15
    //   block 235:    animdata_name = N5S1HTH,          hash = 235,    nb_records =    19
    //   block 236:    animdata_name = N6S1HTH,          hash = 236,    nb_records =    23
    //   block 237:    animdata_name = N7S1HTH,          hash = 237,    nb_records =    37
    //   block 238:    animdata_name = DBS1HTH,          hash = 238,    nb_records =    31
    //   block 239:    animdata_name = DBS2HTH,          hash = 239,    nb_records =    29
    //   block 240:    animdata_name = DDS1HTH,          hash = 240,    nb_records =    30
    //   block 241:    animdata_name = DDS2HTH,          hash = 241,    nb_records =    27
    //   block 242:    animdata_name = DDS3HTH,          hash = 242,    nb_records =    27
    //   block 243:    animdata_name = DDS4HTH,          hash = 243,    nb_records =    30
    //   block 244:    animdata_name = C0ONHTH,          hash = 244,    nb_records =    30
    //   block 245:    animdata_name = D0ONHTH,          hash = 245,    nb_records =    29
    //   block 246:    animdata_name = D1ONHTH,          hash = 246,    nb_records =    34
    //   block 247:    animdata_name = DKS1HTH,          hash = 247,    nb_records =    35
    //   block 248:    animdata_name = GIS1HTH,          hash = 248,    nb_records =    43
    //   block 249:    animdata_name = OBS1HTH,          hash = 249,    nb_records =    50
    //   block 250:    animdata_name = F3ONHTH,          hash = 250,    nb_records =    50
    //   block 251:    animdata_name = H2ONHTH,          hash = 251,    nb_records =    46
    //   block 252:    animdata_name = F5ONHTH,          hash = 252,    nb_records =    46
    //   block 253:    animdata_name = H2OPHTH,          hash = 253,    nb_records =    60
    //   block 254:    animdata_name = L1ONHTH,          hash = 254,    nb_records =    55
    //   block 255:    animdata_name = L2ONHTH,          hash = 255,    nb_records =    54
    //   }}}

    unsigned int    i;
    unsigned int    nb   = strlen(name);
    UBYTE           hash = 0;

	// 找到一个字符串中的最后一个"."所在的位置
    for (i=0; i < strlen(name); i++) {
        if (name[i] == '.') {
            nb = i;
        }
    }
	// 对最后一个"."之前的字符求和, 不包括"."
    // UBYTE           hash = 0;
    // 所以求和会有高位溢出的
    for (i=0; i < nb; i++) {
        hash += toupper(name[i]);
    }
    return hash;
}


// ==========================================================================
void animdata_load(void)
{

#if 0
    D2 is a file format specifically used for animations datas. 
    In fact, there are 2 type of D2 file in the MPQ despite they have the same extension. 
    The first type is mainly used to retrieve the Animation Speed and the Number of Frames per Direction of any animation and is the source of common troubles when making a new animation. 
    The second type of D2 format contains the copy of many COF. 

    These last D2 files are preloaded at the start of the game, so when the game needs to read a COF, it first looks in that D2 files, and if the COF is not here, it’ll looks for the regular one instead.
    
    D2 files of type 1 (animations datas), used by the game:
         Data\Global\AnimData.d2
    D2 files of type 2 (copy of many COF), used by the game:
         Data\Global\chars_cof.d2 Player Characters
         Data\Global\cmncof_a1.d2 COF needed for Act 1
         Data\Global\cmncof_a2.d2 for Act 2
         Data\Global\cmncof_a3.d2 for Act 3
         Data\Global\cmncof_a4.d2 for Act 4
         Data\Global\cmncof_a6.d2 for Act 5 (that’s really cmncof_a6.d2)
                  ·
#endif

    char               * name = "data/global/animdata.d2";
    char				 animdata_name[10];
    ANIM_DATA_RECORD_S * rec_ptr = NULL;
    long               * lptr;

    long                 nb_rec;
    int                  entry, done = FALSE, curr_block = 0;
    UBYTE                hash;


    // free the file if already in mem
    if (glb_ds1edit.anim_data.buffer != NULL) {
        free(glb_ds1edit.anim_data.buffer);
    }

    // load it
    memset( & glb_ds1edit.anim_data, 0, sizeof(glb_ds1edit.anim_data) );
    entry = misc_load_mpq_file( name, (char **) & glb_ds1edit.anim_data.buffer, & glb_ds1edit.anim_data.buffer_size, FALSE );
    if ((entry == -1) || (glb_ds1edit.anim_data.buffer_size <= 4)) {
        printf("error : couldn't load <%s>\n", name);
        return;
    } else {
        printf("ok, <%s> found\n", name);
    }

    // read all blocks and update pointers
    lptr = ((long *) glb_ds1edit.anim_data.buffer);

    while ( !done) {

        nb_rec = * lptr;
        lptr++;
        // 为了调试, 这一句是无所谓的
        animdata_name[0] = '\0';

        if (nb_rec) {

            //   typedef struct ANIM_DATA_RECORD_S {{{
            //   {
            //      char  cof_name[8];
            //      有的地方叫fpd 有的地方叫frames_per_dir
            //      所以我估计是两个人写的
            //      long  frames_per_dir;
            //      long  speed;
            //      UBYTE flags[144];
            //   } ANIM_DATA_RECORD_S;
            //   }}}
            rec_ptr = (ANIM_DATA_RECORD_S *) lptr;
            memset(animdata_name, 0, sizeof(animdata_name));
            strncpy(animdata_name, rec_ptr->cof_name, 8);
            // 通过高位溢出的方式对字符串求和, 进而得到一个hash值
            hash = animdata_hash_value(animdata_name);
        } else {
            hash = 0;
        }

        // debug infos
        printf("   block %3i: animdata_name = %10s, hash = %5i, nb_records = %5li\n", curr_block, animdata_name, hash, nb_rec);

        //   下面是所有算出的hash值
        //   {{{
        //   block   0:    animdata_name = L1OPHTH,          hash =   0,    nb_records =    54
        //   block   1:    animdata_name = L2OPHTH,          hash =   1,    nb_records =    57
        //   block   2:    animdata_name = F9OPHTH,          hash =   2,    nb_records =    53
        //   block   3:    animdata_name = Q1ONHTH,          hash =   3,    nb_records =    42
        //   block   4:    animdata_name = S0ONHTH,          hash =   4,    nb_records =    41
        //   block   5:    animdata_name = S1ONHTH,          hash =   5,    nb_records =    51
        //   block   6:    animdata_name = S2ONHTH,          hash =   6,    nb_records =    54
        //   block   7:    animdata_name = U1ONHTH,          hash =   7,    nb_records =    40
        //   block   8:    animdata_name = U2ONHTH,          hash =   8,    nb_records =    47
        //   block   9:    animdata_name = W1ONHTH,          hash =   9,    nb_records =    62
        //   block  10:    animdata_name = W2ONHTH,          hash =  10,    nb_records =    67
        //   block  11:    animdata_name = Y1ONHTH,          hash =  11,    nb_records =    63
        //   block  12:    animdata_name = Z1ONHTH,          hash =  12,    nb_records =    60
        //   block  13:    animdata_name = Z2ONHTH,          hash =  13,    nb_records =    64
        //   block  14:    animdata_name = Z3ONHTH,          hash =  14,    nb_records =    60
        //   block  15:    animdata_name = Z4ONHTH,          hash =  15,    nb_records =    60
        //   block  16:    animdata_name = Z5ONHTH,          hash =  16,    nb_records =    59
        //   block  17:    animdata_name = Z4OPHTH,          hash =  17,    nb_records =    62
        //   block  18:    animdata_name = Z5OPHTH,          hash =  18,    nb_records =    58
        //   block  19:    animdata_name = Z2NUHTH,          hash =  19,    nb_records =    57
        //   block  20:    animdata_name = Z3NUHTH,          hash =  20,    nb_records =    48
        //   block  21:    animdata_name = Z4NUHTH,          hash =  21,    nb_records =    51
        //   block  22:    animdata_name = Z5NUHTH,          hash =  22,    nb_records =    45
        //   block  23:    animdata_name = Y7NUHTH,          hash =  23,    nb_records =    38
        //   block  24:    animdata_name = Y8NUHTH,          hash =  24,    nb_records =    34
        //   block  25:    animdata_name = Y9NUHTH,          hash =  25,    nb_records =    40
        //   block  26:    animdata_name = UBOPHTH,          hash =  26,    nb_records =    45
        //   block  27:    animdata_name = YAONHTH,          hash =  27,    nb_records =    35
        //   block  28:    animdata_name = ZAONHTH,          hash =  28,    nb_records =    37
        //   block  29:    animdata_name = YCONHTH,          hash =  29,    nb_records =    54
        //   block  30:    animdata_name = ZCONHTH,          hash =  30,    nb_records =    53
        //   block  31:    animdata_name = ZDONHTH,          hash =  31,    nb_records =    41
        //   block  32:    animdata_name = ZEONHTH,          hash =  32,    nb_records =    50
        //   block  33:    animdata_name = ZDOPHTH,          hash =  33,    nb_records =    43
        //   block  34:    animdata_name = ZEOPHTH,          hash =  34,    nb_records =    47
        //   block  35:    animdata_name = ZHONHTH,          hash =  35,    nb_records =    45
        //   block  36:    animdata_name = ZCNUHTH,          hash =  36,    nb_records =    41
        //   block  37:    animdata_name = ZHOPHTH,          hash =  37,    nb_records =    41
        //   block  38:    animdata_name = ZENUHTH,          hash =  38,    nb_records =    36
        //   block  39:    animdata_name = ZLONHTH,          hash =  39,    nb_records =    36
        //   block  40:    animdata_name = YNONHTH,          hash =  40,    nb_records =    31
        //   block  41:    animdata_name = ZLOPHTH,          hash =  41,    nb_records =    38
        //   block  42:    animdata_name = YPONHTH,          hash =  42,    nb_records =    29
        //   block  43:    animdata_name = YQONHTH,          hash =  43,    nb_records =    19
        //   block  44:    animdata_name = ZQONHTH,          hash =  44,    nb_records =    20
        //   block  45:    animdata_name = ZRONHTH,          hash =  45,    nb_records =    23
        //   block  46:    animdata_name = ZSONHTH,          hash =  46,    nb_records =    26
        //   block  47:    animdata_name = ZROPHTH,          hash =  47,    nb_records =    16
        //   block  48:    animdata_name = ZSOPHTH,          hash =  48,    nb_records =    17
        //   block  49:    animdata_name = ZVONHTH,          hash =  49,    nb_records =    18
        //   block  50:    animdata_name = ZWONHTH,          hash =  50,    nb_records =    17
        //   block  51:    animdata_name = ZVOPHTH,          hash =  51,    nb_records =    12
        //   block  52:    animdata_name = ZYONHTH,          hash =  52,    nb_records =    15
        //   block  53:    animdata_name = YYOPHTH,          hash =  53,    nb_records =    11
        //   block  54:    animdata_name = ZYOPHTH,          hash =  54,    nb_records =    10
        //   block  55:    animdata_name = ZVNUHTH,          hash =  55,    nb_records =     5
        //   block  56:    animdata_name = ZWNUHTH,          hash =  56,    nb_records =     6
        //   block  57:    animdata_name = YYNUHTH,          hash =  57,    nb_records =     3
        //   block  58:    animdata_name = ZYNUHTH,          hash =  58,    nb_records =     4
        //   block  59:    animdata_name = ZZWLHTH,          hash =  59,    nb_records =     2
        //   block  60:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  61:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  62:    animdata_name = SOTWXBW,          hash =  62,    nb_records =     1
        //   block  63:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  64:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  65:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  66:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  67:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  68:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  69:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  70:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  71:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  72:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  73:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  74:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  75:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  76:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  77:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  78:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  79:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  80:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  81:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  82:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  83:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  84:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  85:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  86:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  87:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  88:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  89:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  90:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  91:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  92:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  93:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  94:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  95:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  96:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  97:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  98:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block  99:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 100:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 101:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 102:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 103:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 104:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 105:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 106:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 107:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 108:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 109:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 110:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 111:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 112:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 113:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 114:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 115:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 116:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 117:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 118:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 119:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 120:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 121:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 122:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 123:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 124:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 125:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 126:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 127:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 128:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 129:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 130:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 131:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 132:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 133:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 134:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 135:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 136:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 137:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 138:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 139:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 140:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 141:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 142:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 143:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 144:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 145:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 146:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 147:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 148:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 149:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 150:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 151:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 152:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 153:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 154:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 155:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 156:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 157:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 158:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 159:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 160:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 161:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 162:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 163:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 164:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 165:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 166:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 167:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 168:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 169:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 170:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 171:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 172:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 173:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 174:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 175:    animdata_name = 0AA11HS,          hash = 175,    nb_records =     1
        //   block 176:    animdata_name = 0AA21HS,          hash = 176,    nb_records =     2
        //   block 177:    animdata_name = 0AA22HS,          hash = 177,    nb_records =     1
        //   block 178:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 179:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 180:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 181:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 182:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 183:    animdata_name = 10A1HTH,          hash = 183,    nb_records =     1
        //   block 184:    animdata_name = 10A2HTH,          hash = 184,    nb_records =     1
        //   block 185:    animdata_name = 21A1HTH,          hash = 185,    nb_records =     1
        //   block 186:    animdata_name = 40A1HTH,          hash = 186,    nb_records =     3
        //   block 187:    animdata_name = 40A2HTH,          hash = 187,    nb_records =     3
        //   block 188:    animdata_name = 15A1HTH,          hash = 188,    nb_records =     2
        //   block 189:    animdata_name = 43A1HTH,          hash = 189,    nb_records =     2
        //   block 190:    animdata_name = 44A1HTH,          hash = 190,    nb_records =     2
        //   block 191:    animdata_name = 45A1HTH,          hash = 191,    nb_records =     1
        //   block 192:    animdata_name = 64A1HTH,          hash = 192,    nb_records =     3
        //   block 193:    animdata_name = 65A1HTH,          hash = 193,    nb_records =     2
        //   block 194:    animdata_name = BAA21HS,          hash = 194,    nb_records =     3
        //   block 195:    animdata_name = BAA22HS,          hash = 195,    nb_records =     4
        //   block 196:    animdata_name = BAA22HT,          hash = 196,    nb_records =     3
        //   block 197:    animdata_name = 0ADD1HS,          hash = 197,    nb_records =     2
        //   block 198:    animdata_name = 0ADD2HS,          hash = 198,    nb_records =     1
        //   block 199:    animdata_name =        ,          hash =   0,    nb_records =     0
        //   block 200:    animdata_name = 0BA1HTH,          hash = 200,    nb_records =     2
        //   block 201:    animdata_name = FEA11HS,          hash = 201,    nb_records =     7
        //   block 202:    animdata_name = 20S1HTH,          hash = 202,    nb_records =     8
        //   block 203:    animdata_name = 10S3HTH,          hash = 203,    nb_records =     5
        //   block 204:    animdata_name = F0A1HTH,          hash = 204,    nb_records =     9
        //   block 205:    animdata_name = F0A2HTH,          hash = 205,    nb_records =    10
        //   block 206:    animdata_name = 60S1HTH,          hash = 206,    nb_records =    11
        //   block 207:    animdata_name = 61S1HTH,          hash = 207,    nb_records =    13
        //   block 208:    animdata_name = 62S1HTH,          hash = 208,    nb_records =    14
        //   block 209:    animdata_name = 62S2HTH,          hash = 209,    nb_records =    10
        //   block 210:    animdata_name = 15DDHTH,          hash = 210,    nb_records =     7
        //   block 211:    animdata_name = GMA12HS,          hash = 211,    nb_records =     7
        //   block 212:    animdata_name = GMA22HS,          hash = 212,    nb_records =     9
        //   block 213:    animdata_name = L3A1HTH,          hash = 213,    nb_records =     8
        //   block 214:    animdata_name = L3A2HTH,          hash = 214,    nb_records =     7
        //   block 215:    animdata_name = M4A1HTH,          hash = 215,    nb_records =     6
        //   block 216:    animdata_name = M5A1HTH,          hash = 216,    nb_records =     7
        //   block 217:    animdata_name = M6A1HTH,          hash = 217,    nb_records =     7
        //   block 218:    animdata_name = K9A1HTH,          hash = 218,    nb_records =     5
        //   block 219:    animdata_name = K9A2HTH,          hash = 219,    nb_records =    10
        //   block 220:    animdata_name = D0S1HTH,          hash = 220,    nb_records =    12
        //   block 221:    animdata_name = D1S1HTH,          hash = 221,    nb_records =    20
        //   block 222:    animdata_name = E1S1HTH,          hash = 222,    nb_records =    21
        //   block 223:    animdata_name = E2S1HTH,          hash = 223,    nb_records =    19
        //   block 224:    animdata_name = D4S1HTH,          hash = 224,    nb_records =    21
        //   block 225:    animdata_name = D5S1HTH,          hash = 225,    nb_records =    22
        //   block 226:    animdata_name = D6S1HTH,          hash = 226,    nb_records =    20
        //   block 227:    animdata_name = D7S1HTH,          hash = 227,    nb_records =    17
        //   block 228:    animdata_name = D8S1HTH,          hash = 228,    nb_records =    15
        //   block 229:    animdata_name = D9S1HTH,          hash = 229,    nb_records =    19
        //   block 230:    animdata_name = D9S2HTH,          hash = 230,    nb_records =    26
        //   block 231:    animdata_name = D9S3HTH,          hash = 231,    nb_records =    19
        //   block 232:    animdata_name = D9S4HTH,          hash = 232,    nb_records =    16
        //   block 233:    animdata_name = 62ONHTH,          hash = 233,    nb_records =    17
        //   block 234:    animdata_name = 70OPHTH,          hash = 234,    nb_records =    15
        //   block 235:    animdata_name = N5S1HTH,          hash = 235,    nb_records =    19
        //   block 236:    animdata_name = N6S1HTH,          hash = 236,    nb_records =    23
        //   block 237:    animdata_name = N7S1HTH,          hash = 237,    nb_records =    37
        //   block 238:    animdata_name = DBS1HTH,          hash = 238,    nb_records =    31
        //   block 239:    animdata_name = DBS2HTH,          hash = 239,    nb_records =    29
        //   block 240:    animdata_name = DDS1HTH,          hash = 240,    nb_records =    30
        //   block 241:    animdata_name = DDS2HTH,          hash = 241,    nb_records =    27
        //   block 242:    animdata_name = DDS3HTH,          hash = 242,    nb_records =    27
        //   block 243:    animdata_name = DDS4HTH,          hash = 243,    nb_records =    30
        //   block 244:    animdata_name = C0ONHTH,          hash = 244,    nb_records =    30
        //   block 245:    animdata_name = D0ONHTH,          hash = 245,    nb_records =    29
        //   block 246:    animdata_name = D1ONHTH,          hash = 246,    nb_records =    34
        //   block 247:    animdata_name = DKS1HTH,          hash = 247,    nb_records =    35
        //   block 248:    animdata_name = GIS1HTH,          hash = 248,    nb_records =    43
        //   block 249:    animdata_name = OBS1HTH,          hash = 249,    nb_records =    50
        //   block 250:    animdata_name = F3ONHTH,          hash = 250,    nb_records =    50
        //   block 251:    animdata_name = H2ONHTH,          hash = 251,    nb_records =    46
        //   block 252:    animdata_name = F5ONHTH,          hash = 252,    nb_records =    46
        //   block 253:    animdata_name = H2OPHTH,          hash = 253,    nb_records =    60
        //   block 254:    animdata_name = L1ONHTH,          hash = 254,    nb_records =    55
        //   block 255:    animdata_name = L2ONHTH,          hash = 255,    nb_records =    54
        //   }}}

        // printf("   block %3i, hash = %5i, nb_records = %5li\n", curr_block, hash, nb_rec);
        fflush(stdout);
        curr_block++;

        // update block pointer & records number
        if (nb_rec) {
            // rec_ptr = (ANIM_DATA_RECORD_S *) lptr;
            // 一个平面结构, 序列中是ANIM_DATA_RECORD_S的不断写入
            glb_ds1edit.anim_data.block_pointer[hash] = rec_ptr;
            glb_ds1edit.anim_data.nb_records[hash]    = nb_rec;
        }

        // next block, if any
        // skip nb_rec records of 160 bytes each
        // why
        lptr += 40 * nb_rec;
        if ( (UBYTE *) lptr >= ((UBYTE *) glb_ds1edit.anim_data.buffer + glb_ds1edit.anim_data.buffer_size) ) {
            done = TRUE;
        }
    }
    printf("\n");
}


// ==========================================================================
// 'name' is the cof name, WITHOUT the 'data\global\...' in front.
// The ".cof" extension can be in the name, but it won't be used
int animdata_get_cof_info(char * name, long * fpd, long * speed)
{
//	typedef struct ANIM_DATA_RECORD_S {
//      char  cof_name[8];
//      long  frames_per_dir;
//      long  speed;
//      UBYTE flags[144];
//   } ANIM_DATA_RECORD_S;

   ANIM_DATA_RECORD_S * rec_ptr;
   UBYTE              hash;
   int                i;
   char               rec_name[80];


   hash = animdata_hash_value(name);
   printf("%s is hash %i", name, hash);
   if (glb_ds1edit.anim_data.block_pointer[hash]) {
      rec_ptr = glb_ds1edit.anim_data.block_pointer[hash];
      for (i=0; i < glb_ds1edit.anim_data.nb_records[hash]; i++) {
         memset(rec_name, 0, sizeof(rec_name));
         strncpy(rec_name, rec_ptr[i].cof_name, 8);
         if (stricmp(rec_name, name) == 0) {
            * fpd   = rec_ptr[i].frames_per_dir;
            * speed = rec_ptr[i].speed;
            printf(", record %i in that block\n", i);
            fflush(stdout);
            return 0;
         }
      }
   }
   * fpd   = 0;
   * speed = 0;
   printf(", but error : not found in there\n");
   fflush(stdout);
   return -1;
}
