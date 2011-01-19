/*
 *  SequenceHashArbitrary_Builder.hpp
 *  bleu
 *
 *  Created by Rosangela Canino-Koning on 12/27/10.
 *
 */

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "SequenceHashArbitrary_Lookup.hpp"

// test validity
#define is_valid_dna_character(ch) ( ch == 'A' || ch == 'a' || \
ch == 'C' || ch == 'c' || \
ch == 'G' || ch == 'g' || \
ch == 'T' || ch == 't' )

#define twobit_representation(ch) ( ( ch == 'A' || ch == 'a' ) ? 0LL : \
( ch == 'C' || ch == 'c' ) ? 1LL : \
( ch == 'G' || ch == 'g' ) ? 2LL : 3LL )

#define reversetwobit_representation(n) ((n) == 0 ? 'A' : \
(n) == 1 ? 'C' : \
(n) == 2 ? 'G' : 'T')

#define complement_twobit(n) ((~n & 3))

#define twobit_complement(ch) ( ( ch == 'A' || ch == 'a' ) ? 3LL : \
( ch == 'T' || ch == 't' ) ? 0LL : \
( ch == 'C' || ch == 'c' ) ? 2LL : 1LL)

using namespace std;

namespace bleu {
  class SequenceHashArbitrary_Builder {
    private:
      SequenceHashArbitrary_Lookup _lookup;
      
      static const unsigned long long PrimePowers[200][4];

    private:
      unsigned long long rotatebitsclockwise( unsigned long long aHash )
      {
        return ( aHash >> 1 ) | ( aHash << 63 );
      }

    public:
      
    // takes a sequence and computes the forward and reverse strand hashes, then
    // picks the lower value as the canonical hash.
    //
    // each position is converted to the corresponding prime number (in Primes[])
    // and raised to the nucleotide's power (Nucleotides[]).
    // then each position is multiplied successively with each other position,
    // letting the extra bits fall off the end.
    // The total is bit-rotated clockwise after each multiplication.
    // finally the corresponding hashes are uniquified.
    unsigned long long hash(string & aSeq)
    {
      const char * lSeq = aSeq.c_str();
      int lLength = aSeq.length();
      
      unsigned long long lHash = 1;
      unsigned long long lHashRev = 1;

      for (int i = 0, j = lLength - 1; i < lLength; ++i, --j)
      {
        // good way

        lHash *= PrimePowers[i][twobit_representation(lSeq[i])];
        lHashRev *= PrimePowers[i][twobit_complement(lSeq[j])];
                       
        lHash = rotatebitsclockwise( lHash );
        lHashRev = rotatebitsclockwise( lHashRev );
      }
      
      return lHash ^ lHashRev;
    }
    
//    unsigned long long hash(string & aSeq)
//    {
//      int lLength = aSeq.length();
//
//      unsigned long long lHash = 1;
//      unsigned long long lHashRev = 1;
//
//      for ( int i = 0, j = 0; i < lLength; i += CHUNK_SIZE, ++j )
//      {
//        unsigned long long lChunk = 0;
//        unsigned long long lChunkRev = 0;
//
//        for (int l = 0; l < CHUNK_SIZE; ++l)
//        {
//          if ( l > 0 )
//          {
//            lChunk <<= 2;
//            lChunkRev <<= 2;
//          }
//          
//          if ( i + l < lLength ) 
//          {
//            unsigned char lTwoBitRep = twobit_representation(aSeq[i+l]);
//            
//            lChunk += lTwoBitRep;
//            lChunkRev += complement_twobit(lTwoBitRep); 
//          }
//        }
//        
//        lHash *= _lookup.PrimesLookup[j][ lChunk ];
//        lHashRev *= _lookup.PrimesLookup[j][ lChunkRev ];
//              
//        lHash = rotatebitsclockwise( lHash );
//        lHashRev = rotatebitsclockwise( lHashRev );
//      }
//      
//      return lHash ^ lHashRev;
//    }

  };
  
  const unsigned long long SequenceHashArbitrary_Builder::PrimePowers[200][4] =
  {
    { 3, 9, 27, 81 }, 
    { 5, 25, 125, 625 }, 
    { 7, 49, 343, 2401 }, 
    { 11, 121, 1331, 14641 }, 
    { 13, 169, 2197, 28561 }, 
    { 17, 289, 4913, 83521 }, 
    { 19, 361, 6859, 130321 }, 
    { 23, 529, 12167, 279841 }, 
    { 29, 841, 24389, 707281 }, 
    { 31, 961, 29791, 923521 }, 
    { 37, 1369, 50653, 1874161 }, 
    { 41, 1681, 68921, 2825761 }, 
    { 43, 1849, 79507, 3418801 }, 
    { 47, 2209, 103823, 4879681 }, 
    { 53, 2809, 148877, 7890481 }, 
    { 59, 3481, 205379, 12117361 }, 
    { 61, 3721, 226981, 13845841 }, 
    { 67, 4489, 300763, 20151121 }, 
    { 71, 5041, 357911, 25411681 }, 
    { 73, 5329, 389017, 28398241 }, 
    { 79, 6241, 493039, 38950081 }, 
    { 83, 6889, 571787, 47458321 }, 
    { 89, 7921, 704969, 62742241 }, 
    { 97, 9409, 912673, 88529281 }, 
    { 101, 10201, 1030301, 104060401 }, 
    { 103, 10609, 1092727, 112550881 }, 
    { 107, 11449, 1225043, 131079601 }, 
    { 109, 11881, 1295029, 141158161 }, 
    { 113, 12769, 1442897, 163047361 }, 
    { 127, 16129, 2048383, 260144641 }, 
    { 131, 17161, 2248091, 294499921 }, 
    { 137, 18769, 2571353, 352275361 }, 
    { 139, 19321, 2685619, 373301041 }, 
    { 149, 22201, 3307949, 492884401 }, 
    { 151, 22801, 3442951, 519885601 }, 
    { 157, 24649, 3869893, 607573201 }, 
    { 163, 26569, 4330747, 705911761 }, 
    { 167, 27889, 4657463, 777796321 }, 
    { 173, 29929, 5177717, 895745041 }, 
    { 179, 32041, 5735339, 1026625681 }, 
    { 181, 32761, 5929741, 1073283121 }, 
    { 191, 36481, 6967871, 1330863361 }, 
    { 193, 37249, 7189057, 1387488001 }, 
    { 197, 38809, 7645373, 1506138481 }, 
    { 199, 39601, 7880599, 1568239201 }, 
    { 211, 44521, 9393931, 1982119441 }, 
    { 223, 49729, 11089567, 2472973441ULL }, 
    { 227, 51529, 11697083, 2655237841ULL }, 
    { 229, 52441, 12008989, 2750058481ULL }, 
    { 233, 54289, 12649337, 2947295521ULL }, 
    { 239, 57121, 13651919, 3262808641ULL }, 
    { 241, 58081, 13997521, 3373402561ULL }, 
    { 251, 63001, 15813251, 3969126001ULL }, 
    { 257, 66049, 16974593, 4362470401ULL }, 
    { 263, 69169, 18191447, 4784350561ULL }, 
    { 269, 72361, 19465109, 5236114321ULL }, 
    { 271, 73441, 19902511, 5393580481ULL }, 
    { 277, 76729, 21253933, 5887339441ULL }, 
    { 281, 78961, 22188041, 6234839521ULL }, 
    { 283, 80089, 22665187, 6414247921ULL }, 
    { 293, 85849, 25153757, 7370050801ULL }, 
    { 307, 94249, 28934443, 8882874001ULL }, 
    { 311, 96721, 30080231, 9354951841ULL }, 
    { 313, 97969, 30664297, 9597924961ULL }, 
    { 317, 100489, 31855013, 10098039121ULL }, 
    { 331, 109561, 36264691, 12003612721ULL }, 
    { 337, 113569, 38272753, 12897917761ULL }, 
    { 347, 120409, 41781923, 14498327281ULL }, 
    { 349, 121801, 42508549, 14835483601ULL }, 
    { 353, 124609, 43986977, 15527402881ULL }, 
    { 359, 128881, 46268279, 16610312161ULL }, 
    { 367, 134689, 49430863, 18141126721ULL }, 
    { 373, 139129, 51895117, 19356878641ULL }, 
    { 379, 143641, 54439939, 20632736881ULL }, 
    { 383, 146689, 56181887, 21517662721ULL }, 
    { 389, 151321, 58863869, 22898045041ULL }, 
    { 397, 157609, 62570773, 24840596881ULL }, 
    { 401, 160801, 64481201, 25856961601ULL }, 
    { 409, 167281, 68417929, 27982932961ULL }, 
    { 419, 175561, 73560059, 30821664721ULL }, 
    { 421, 177241, 74618461, 31414372081ULL }, 
    { 431, 185761, 80062991, 34507149121ULL }, 
    { 433, 187489, 81182737, 35152125121ULL }, 
    { 439, 192721, 84604519, 37141383841ULL }, 
    { 443, 196249, 86938307, 38513670001ULL }, 
    { 449, 201601, 90518849, 40642963201ULL }, 
    { 457, 208849, 95443993, 43617904801ULL }, 
    { 461, 212521, 97972181, 45165175441ULL }, 
    { 463, 214369, 99252847, 45954068161ULL }, 
    { 467, 218089, 101847563, 47562811921ULL }, 
    { 479, 229441, 109902239, 52643172481ULL }, 
    { 487, 237169, 115501303, 56249134561ULL }, 
    { 491, 241081, 118370771, 58120048561ULL }, 
    { 499, 249001, 124251499, 62001498001ULL }, 
    { 503, 253009, 127263527, 64013554081ULL }, 
    { 509, 259081, 131872229, 67122964561ULL }, 
    { 521, 271441, 141420761, 73680216481ULL }, 
    { 523, 273529, 143055667, 74818113841ULL }, 
    { 541, 292681, 158340421, 85662167761ULL }, 
    { 547, 299209, 163667323, 89526025681ULL }, 
    { 557, 310249, 172808693, 96254442001ULL }, 
    { 563, 316969, 178453547, 100469346961ULL }, 
    { 569, 323761, 184220009, 104821185121ULL }, 
    { 571, 326041, 186169411, 106302733681ULL }, 
    { 577, 332929, 192100033, 110841719041ULL }, 
    { 587, 344569, 202262003, 118727795761ULL }, 
    { 593, 351649, 208527857, 123657019201ULL }, 
    { 599, 358801, 214921799, 128738157601ULL }, 
    { 601, 361201, 217081801, 130466162401ULL }, 
    { 607, 368449, 223648543, 135754665601ULL }, 
    { 613, 375769, 230346397, 141202341361ULL }, 
    { 617, 380689, 234885113, 144924114721ULL }, 
    { 619, 383161, 237176659, 146812351921ULL }, 
    { 631, 398161, 251239591, 158532181921ULL }, 
    { 641, 410881, 263374721, 168823196161ULL }, 
    { 643, 413449, 265847707, 170940075601ULL }, 
    { 647, 418609, 270840023, 175233494881ULL }, 
    { 653, 426409, 278445077, 181824635281ULL }, 
    { 659, 434281, 286191179, 188599986961ULL }, 
    { 661, 436921, 288804781, 190899960241ULL }, 
    { 673, 452929, 304821217, 205144679041ULL }, 
    { 677, 458329, 310288733, 210065472241ULL }, 
    { 683, 466489, 318611987, 217611987121ULL }, 
    { 691, 477481, 329939371, 227988105361ULL }, 
    { 701, 491401, 344472101, 241474942801ULL }, 
    { 709, 502681, 356400829, 252688187761ULL }, 
    { 719, 516961, 371694959, 267248675521ULL }, 
    { 727, 528529, 384240583, 279342903841ULL }, 
    { 733, 537289, 393832837, 288679469521ULL }, 
    { 739, 546121, 403583419, 298248146641ULL }, 
    { 743, 552049, 410172407, 304758098401ULL }, 
    { 751, 564001, 423564751, 318097128001ULL }, 
    { 757, 573049, 433798093, 328385156401ULL }, 
    { 761, 579121, 440711081, 335381132641ULL }, 
    { 769, 591361, 454756609, 349707832321ULL }, 
    { 773, 597529, 461889917, 357040905841ULL }, 
    { 787, 619369, 487443403, 383617958161ULL }, 
    { 797, 635209, 506261573, 403490473681ULL }, 
    { 809, 654481, 529475129, 428345379361ULL }, 
    { 811, 657721, 533411731, 432596913841ULL }, 
    { 821, 674041, 553387661, 454331269681ULL }, 
    { 823, 677329, 557441767, 458774574241ULL }, 
    { 827, 683929, 565609283, 467758877041ULL }, 
    { 829, 687241, 569722789, 472300192081ULL }, 
    { 839, 703921, 590589719, 495504774241ULL }, 
    { 853, 727609, 620650477, 529414856881ULL }, 
    { 857, 734449, 629422793, 539415333601ULL }, 
    { 859, 737881, 633839779, 544468370161ULL }, 
    { 863, 744769, 642735647, 554680863361ULL }, 
    { 877, 769129, 674526133, 591559418641ULL }, 
    { 881, 776161, 683797841, 602425897921ULL }, 
    { 883, 779689, 688465387, 607914936721ULL }, 
    { 887, 786769, 697864103, 619005459361ULL }, 
    { 907, 822649, 746142643, 676751377201ULL }, 
    { 911, 829921, 756058031, 688768866241ULL }, 
    { 919, 844561, 776151559, 713283282721ULL }, 
    { 929, 863041, 801765089, 744839767681ULL }, 
    { 937, 877969, 822656953, 770829564961ULL }, 
    { 941, 885481, 833237621, 784076601361ULL }, 
    { 947, 896809, 849278123, 804266382481ULL }, 
    { 953, 908209, 865523177, 824843587681ULL }, 
    { 967, 935089, 904231063, 874391437921ULL }, 
    { 971, 942841, 915498611, 888949151281ULL }, 
    { 977, 954529, 932574833, 911125611841ULL }, 
    { 983, 966289, 949862087, 933714431521ULL }, 
    { 991, 982081, 973242271, 964483090561ULL }, 
    { 997, 994009, 991026973, 988053892081ULL }, 
    { 1009, 1018081, 1027243729, 1036488922561ULL }, 
    { 1013, 1026169, 1039509197, 1053022816561ULL }, 
    { 1019, 1038361, 1058089859, 1078193566321ULL }, 
    { 1021, 1042441, 1064332261, 1086683238481ULL }, 
    { 1031, 1062961, 1095912791, 1129886087521ULL }, 
    { 1033, 1067089, 1102302937, 1138678933921ULL }, 
    { 1039, 1079521, 1121622319, 1165365589441ULL }, 
    { 1049, 1100401, 1154320649, 1210882360801ULL }, 
    { 1051, 1104601, 1160935651, 1220143369201ULL }, 
    { 1061, 1125721, 1194389981, 1267247769841ULL }, 
    { 1063, 1129969, 1201157047, 1276829940961ULL }, 
    { 1069, 1142761, 1221611509, 1305902703121ULL }, 
    { 1087, 1181569, 1284365503, 1396105301761ULL }, 
    { 1091, 1190281, 1298596571, 1416768858961ULL }, 
    { 1093, 1194649, 1305751357, 1427186233201ULL }, 
    { 1097, 1203409, 1320139673, 1448193221281ULL }, 
    { 1103, 1216609, 1341919727, 1480137458881ULL }, 
    { 1109, 1229881, 1363938029, 1512607274161ULL }, 
    { 1117, 1247689, 1393668613, 1556727840721ULL }, 
    { 1123, 1261129, 1416247867, 1590446354641ULL }, 
    { 1129, 1274641, 1439069689, 1624709678881ULL }, 
    { 1151, 1324801, 1524845951, 1755097689601ULL }, 
    { 1153, 1329409, 1532808577, 1767328289281ULL }, 
    { 1163, 1352569, 1573037747, 1829442899761ULL }, 
    { 1171, 1371241, 1605723211, 1880301880081ULL }, 
    { 1181, 1394761, 1647212741, 1945358247121ULL }, 
    { 1187, 1408969, 1672446203, 1985193642961ULL }, 
    { 1193, 1423249, 1697936057, 2025637716001ULL }, 
    { 1201, 1442401, 1732323601, 2080520644801ULL }, 
    { 1213, 1471369, 1784770597, 2164926734161ULL }, 
    { 1217, 1481089, 1802485313, 2193624625921ULL }, 
    { 1223, 1495729, 1829276567, 2237205241441ULL },
    { 1229, 1510441, 1856331989, 2281432014481ULL } 
  };
}
