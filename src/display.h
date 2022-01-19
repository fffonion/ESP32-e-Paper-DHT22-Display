#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>

#include <Preferences.h>

void InitialiseDisplay();

void DrawMainSection(float t, float h);
void DrawRSSISection(int rssi);

void DrawStatistics(float t, float h, int intervalMinutes);

void DisplaySleep();

const uint8_t u8g2_font_streamline_weather_t[586] U8G2_FONT_SECTION("u8g2_font_streamline_weather_t") =
    "\12\0\4\2\5\5\3\3\6\25\25\0\0\25\0\25\0\0\0\0\0\2-\60.U\322\265\347\1\241\332"
    "\134\36\260\36\334j\6\313\334 \241J\16\223\253A\60S\24K%ba=H\17\322\203\364\230H\36"
    "\21\32|\220\1\61\63\224\262\365C\362\230Ej \213\251\244\311`j\60I\205\322\241l$\24]\204"
    "\312\42\241\314\60T\31\12\306B\361\324`T\31U\251\7\242<f\224\16\1\62N\265\222\265Gj\23"
    "\221RQ$\32\312Db\302HJ\25K\305\62\251H<\24\32D\62\251\314`\230\310\14\42\222L$"
    "\62\30$D\231\310`\240\10e\42\203\201\42\23\212\14\16\62\241\314`\240\211e\6\243Xh\220\21&"
    "\263#=`\220\5\63-\61\332\265\15\244*]\62\225\221eB\232\222t\42\222\323H\65\12MP\241"
    "\214\350\1\211\234(\222\311\210\62\331T&\225SI\7\62\0\64<\224\262\265\17\304\253\340\60\246M\245"
    "C\311\301&\67\270I\15\16$\251\301*\22\32\350\42\241A\60\21\32$\23\241\325@\22\32\305B\243"
    "\134h\224\13\215r\241Q.\64\12\16\16\204\0\65-\257\236\265\212\7\271\301Uj\20\312\251\22\231D"
    ",\27\213Lr\251\134U\62\223*\11\215\22\261\230Z\235H\225\204F\231P\225\341@\5\66\61\256\236"
    "\365h#\321H\64\22\32DJ#\321H\64\22\32DJ#\321At\20\32D\6\321Ar\260\33("
    "d\203El\260\210\15\6\272\301r\20\4\67\65\263\226\65e\63IuF\225\207Ic\252dj\60\10"
    "\231$\42\66]\205\62\241\232T\246(\25\212\204R\241\310\340\3i\36\223\307\344\61yL\36\20\311#"
    "\244\0\70:\265\222\265\346Qy\310`\20\326)\263\271P&\224\12\245B\255B\231P.\24\11\345B"
    "\211<H\42\32I\24\221\212\222\212\42ID\224\315\243\362\250<*\217\312C\352\61Z\0\71F\265\222"
    "u\17D\272I\23Q\273A&\42Ie*\6\211\314\246b\220\210\324T\14\22\221\232\212A\42\263\251"
    "\30$R\231\212A\42\66\310D$\311P\315jR\242\311\15\224\273<*\217J\217\262\232E\36\225G"
    "\205\0\0\0\0\4\377\377\0";

const uint8_t u8g2_font_streamline_ecology_t[1540] U8G2_FONT_SECTION("u8g2_font_streamline_ecology_t") =
    "\26\0\4\2\5\5\3\3\6\25\25\0\0\25\0\25\0\4\207\0\0\5\347\60G\265\222\365\3\356\212r"
    "\252Pp\224\211\15\6\221LJ\263\211\14\42)M,\222\12m\42\351H&\34\11\205\23\341I\42\225"
    "Rdd\31M\66\222\12\245\42\21IF\25Id\62\33Q&\63\30\204\24\253=H\217\312\1\61\62"
    "\25\362u\356\21\32\261&\243\314\346\202\223TD\222J\244R\273P,\230\211G\22\231\134d\42\231f"
    "\64\221\312Dn\24\251\11e\64\31\265l\360 \4\62G\263\226u\216\25\231\250\42\225\34$\42R\221"
    "\42\233\31\244\66!U&\222\212(\62\221D&\21Q\224$\62\211\210\242$\245H\210\22\231DHR"
    "\224h\222\313DB\221Lf\226L\216\22\341L\42\234\12'\62\341T\12\63L\225\262\365\14\366\200P"
    "x\20\32$\243\311h\62\61\30(\222\321d\64\267\30\14\24\261\204b\60Pd&b\225b\60\220\204"
    "\42\212\301 \22\212$\222\31M\42\61XdB\221\304`\21IE\22\203E$\224IjD\321Al"
    "p\222\4\64\77\265\222\365C\22yL\42\17\31\350\21\222<b\221G\14\364\220=(:\30d\222\231"
    "\222`\250$\27\252I\65\311\14>I\350\1\11\235\60\241\207(\62BIg\221Df\62\30HtB"
    "\25\0\65K\265\222\365\345A{\210B\17\320\250V\66\65:I&\362\42\23Jd\22\231\242D&\221"
    ")JH\22\241U\42\223He\6\177\220\30L\6\27\25\3\321`\21I\14r\203D&\241T\204r"
    "\253\242\301&\247\30\34D\7\347\3!\0\66F\265\222\265\15\344\232A\66\65P\344B\221EYD\243"
    "(\33&r\223P\242l\220JDb\3\211\42\23\33\210\352\22\302\252M\62\61\30\11\7\302\344 \67"
    "\31\10\7\3\361`\240\20\16\216\6\241A:\227\36\14b\0\67T\265\222u\205D\61Id\222\211D"
    "\64\233H(\61\271\321,,&\32\305 \61H$\66\22\335 R\64\310\14\22\3\311F\263\230t\62"
    "QD\62\25\223\232\222*M(\241H%\312\22\271HF\224KE\62\301T$\221LE\22\321L("
    "\254\210\350\1\203$\0\70D\225\262\365c\362\200Tl\226I\4S\212\324@\21\212\244b\231P&\65"
    "H\210$;E\244F\22Q\14b\211E\305 \26L\354\202\203H*\27Ke\42\271T&\21Le"
    "\242\261\314,\30\215\312\324\203)\0\71J\265\222\365#\352!\3=d\242\7d\22yLd\246\20\245"
    "\64\203A&\22\212\14\6\241H(\62Pe\64\221](\222IER\211\240&\224\10\15\22!\215D"
    "\25\33DR\211d\42$Ij\64\321\201(\233\10\211#\203x\26:=\265\222\365c\364\230H\36\221"
    "\211\16F\261\201\62$\317\204\226\221Pb\224I\244\26\231P\42\266IieJ]\42\247L\244\244\221"
    "P\70\222\11\207\22\341XV\226\10\352\62\203\201\62\217\2;B\224\262\365\15\306\62\241T\25\17E\202"
    "\221Ld\66\251\30\210\6\212\222\201h \221\14\42\222AD\62H\14\22\203\210n\220\23\332c\22\261"
    "A,\222\32\254\62\231\301@\23\12\15F)\251P&\36\354\0<=\223\266\265\347\21\231<`\236X"
    "\304W\231T&\225\10\326\3\24\11\215Ib\221I\64i\23\251\31\250\6\262dJ\221\320(\22\222\304"
    "\42\223XD\332Dj\6\252\201,\31L\206\6\77\10=D\265\222u\14\362\210\220\36\220\312\3\22\231"
    "\344\244&\246\10E\212\64\241L\244\252&Q\42\211e\24\65\301E$\223^\314\363\240=B\243\316\5"
    "\27\11E$\261)Qe\22Y\231\244J\23\321\203\22\203\37$\0>\67\264\222u\345\1\271\370\36\221"
    "\211\247\262\211T\42\233\212g\362\210y\36:\70\325)\25\16\25\27\272lL\61H\14\22\252\304@\61"
    "P\204\324\232\301\7\322D\36\222\310\1\77E\265\222u\351A\211<&\222\222\310\42\31\211\60\223\226\24"
    "I\24\242LB\42\215\204\65\212\250D\243\307D\6iIF)Y\311\62\211\204*\224\211dD\221P"
    "$\65P\204\62\331\210f:\32\350\21\3=b \4@M\265\222u\15\16\224\331\134\342E,\361\42"
    "\225\7\204\22\7\212E(q\240Xd\42\213\201bR\17\251\70\30$\26%\213\304\42\61Q\14\22\203"
    "Ab\220P\334,\26\212\203\213\205\36\224\30\374 \231\311#\62\351Af\220\314\6\7\17B\0A\62"
    "\260\232\365\251#\331P\62\26\314D\202\11M.\30\13eb)E*\23\13%t\241h&%\251\210"
    "i\42\232\210\42$\32\250\22\42\243\301ATn\7BS\265\222\365\244S\211l\42S\231Id\22\231"
    "\212L\242&\261\310$\62\25\3E&\225H\14\6\211\272\304\340\42\71\70P\16\36\344\6\37&\223\203"
    "\7\271D\277\212\14\16$\231De\242Mh\224ID\62\3M$\223\310\14\64\211\224&\261\321e\6"
    "\232\20\0CU\264\222\265\210\246A\221\244d\220P%j\42\211IQb\220\30D\22\31E$\61\220"
    "-\6\211\301.\21\211'\6\221\301 \25\212$Z\205\42\211\26\203\37$:I\205\22MR\241\301 "
    "\62\210\344R\221\262\330 \223\212E\62\251\330 \23\312EB\231\334 \24\1DC\225\262\65\17\222\61"
    "\205R\244X\16\22\212\355b\240\7\14\324\222\220\62\24\313E\24\241H,\221IH\22\261D\62\21\213"
    "\14NbZYbp\240\210\245\63\221TN\63i\224\10E\32\245b\301L,\23\23EA\224\262\365"
    "\3\22\65\3y*\224h\63\320CR\211.\6\247\241T\242\223\301 \234\313$*\224%\3I(\221"
    "P\14\6\211d\42R\221LD*\222\211\301 \221L\14\6\211d$Q\222\354tp\12\0\0\0\4"
    "\377\377\0";

const uint8_t u8g2_font_streamline_internet_network_t[576] U8G2_FONT_SECTION("u8g2_font_streamline_internet_network_t") =
    "\12\0\4\2\5\5\2\4\6\25\25\0\0\22\0\22\0\0\0\0\0\2#\60H\265\212\65\16\344z\210"
    "t\20\12\253\322\211Pj\240IdR\231U$\264\30\210\22\241Ad \22\15\64#\321@\223\10\211"
    "\6\261\220h\27J\204$\213P$\225\30\250\62\211\314@\25J\244S\342\320 \252\207\310\7B\0\61"
    ")\325\311u\356\21\32u.\233\214FU\353L(\221I\204\22\261TL\225\310$Rz\220p\220\223"
    "\312\42yDh\360A\6\62\60\265\212\65\350A{\320\36t<\311hw\331]v\266\232\214\62e\223"
    "T\42\70T\356\244\63\355J\274\211\204g\203\3\311\36\264\7\355A{\220\0\63.\263\216\65\356\251\366"
    "\350\201\344@\221\333\351v\272\235n\247\32\14R\262\201L\267\23\6\365\20=d\360\377\305\340\3a&"
    "\235\12\17\6\61\0\64/\263\216u\346!{\300@=\30\344\6\231\203Dn\247\333\351v\272\235\36\242"
    "\333\351!\272\235\36\242\207\14\376\277\30| \314\244S\341\301 \6\65\62\263\216\65\356\251\366T{\304"
    "vr\225Z\205R\253Pf\60\310\204B\3Q\325*\24+J\207\6\337\344\1\221< \221\333\351!"
    "\211\301\7\232\301'\0\66\63\264\212\65\15\16\204\331\134\66\227\315e\22\65\271l.\233\313\346\6\17b"
    "\351T:\24\233e\362\200\314\340\3\321\340\343<(\17\312c\226\203\201fp\270\4\67\63\263\216u\346"
    "!{\300@=\30\304\367\210\355\344*\265\12\245V\241t(\265\12\245C\251U(\35\32|\223\7D"
    "\362\200Dn\247\207$\6\37h\6\237\0\70:\265\212\365\16\364\60=,>\312#\62\221\301@\227\321"
    "j\42\322H\211\64R\242\225\211\42\341<*\23\212\205B\203\134(\17\10\15>\20\15>P\15\276\315"
    "\344\1\251\370`\220\4\71Q\265\212\65\16\344\212\12\251$\23\321eR\231Tb\220\32$B\221\310@"
    "R\23\311F\42\203\37$\6\177\240X$\26\211\203D\177\61H\364\27\3Eb\221X$\16~\20\251"
    "\215d\42\221\201\244(\61H\15\22\251L*\223\223d\42RE\205| \4\0\0\0\4\377\377\0";

#endif