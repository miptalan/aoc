d:"0123456789ABCDEF"!(0000b;0001b;0010b;0011b;0100b;0101b;0110b;0111b;1000b;1001b;1010b;1011b;1100b;1101b;1110b;1111b);
toBits:{raze x each y}[d];
pp:{[whole;idx]
    if[not any idx _ whole;
        show"Exhausted packet";
        :enlist (idx;0;0Nj;0Nj);
     ];    
     
    result:();
    version:2 sv (idx;3) sublist whole;
    idx+:3;
    typeId:2 sv (idx;3) sublist whole;
    idx+:3;
    if[typeId=4; / literal
        l:();
        while[first b5:(idx;5) sublist whole;
            idx+:5;
            l,:1_b5
         ];
        l,:1_b5;
        idx+:5;
        
        show "Literal: ",string v:2 sv l;
        result,:enlist (idx;version;typeId;v);
     ];
    if[typeId<>4; / operator
        lengthTypeId:whole idx;
        idx+:1;
        if[not lengthTypeId; / 15 bit of payload length
            len:2 sv (idx;15) sublist whole;
            idx+:15;
            show "Operator type ",string[typeId]," LT=0: will parse ",string[len]," bits";         
            sbpck:();
            midx:idx+len;
            while[idx<midx;
                pck:pp[whole;idx];
                idx:first first pck;
                sbpck,:pck;
             ];
            result,:enlist (idx;version;typeId;sbpck);
          ];
        if[lengthTypeId; / 11 bit of num of sub-packed
            nsp:2 sv (idx;11) sublist whole;
            idx+:11;
            show "Operator type ",string[typeId]," LT=1: will parse ",string[nsp]," sub-packets";
            sbpck:();
            do[nsp;
                pck:pp[whole;idx];
                idx:first first pck;
                sbpck,:pck;
             ];
            result,:enlist (idx;version;typeId;sbpck);
         ];
     ];
    
    result
 };
 
input:first read0 `:input.txt;
sumVersions:{$[0>type last x;x 1;x[1]+sum sumVersions each x 3]};

/part 1
show sumVersions first parsed:pp[;0] toBits input;
types:0 1 2 3 5 6 7!(sum;prd;min;max;>;<;=);
build:{$[7h=type x;x 3;(types x 2;build each x 3)]};

/ part 2
\c 2000 2000
\C 2000 2000
eval build first parsed