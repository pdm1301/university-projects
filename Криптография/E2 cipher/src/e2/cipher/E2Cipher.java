/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package e2.cipher;
import java.io.IOException;
import static java.lang.Math.pow;
import java.math.BigInteger;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.BitSet;
import java.util.Scanner;
/**
 *
 * @author Danil
 */
public class E2Cipher {
    static short s_box[] =
    {
	225, 66, 62, 129, 78, 23, 158, 253, 180, 63, 44, 218, 49, 30, 224, 65,
	204, 243, 130, 125, 124, 18, 142, 187, 228, 88, 21, 213, 111, 233, 76, 75,
	53, 123, 90, 154, 144, 69, 188, 248, 121, 214, 27, 136, 2, 171, 207, 100,
	9, 12, 240, 1, 164, 176, 246, 147, 67, 99, 134, 220, 17, 165, 131, 139,
	201, 208, 25, 149, 106, 161, 92, 36, 110, 80, 33, 128, 47, 231, 83, 15,
	145, 34, 4, 237, 166, 72, 73, 103, 236, 247, 192, 57, 206, 242, 45, 190,
	93, 28, 227, 135, 7, 13, 122, 244, 251, 50, 245, 140, 219, 143, 37, 150,
	168, 234, 205, 51, 101, 84, 6, 141, 137, 10, 94, 217, 22, 14, 113, 108,
	11, 255, 96, 210, 46, 211, 200, 85, 194, 35, 183, 116, 226, 155, 223, 119,
	43, 185, 60, 98, 19, 229, 148, 52, 177, 39, 132, 159, 215, 81, 0, 97,
	173, 133, 115, 3, 8, 64, 239, 104, 254, 151, 31, 222, 175, 102, 232, 184,
	174, 189, 179, 235, 198, 107, 71, 169, 216, 167, 114, 238, 29, 126, 170, 182,
	117, 203, 212, 48, 105, 32, 127, 55, 91, 157, 120, 163, 241, 118, 250, 5,
	61, 58, 68, 87, 59, 202, 199, 138, 24, 70, 156, 191, 186, 56, 86, 26,
	146, 77, 38, 41, 162, 152, 16, 153, 112, 160, 197, 40, 193, 109, 20, 172,
	249, 95, 79, 196, 195, 209, 252, 221, 178, 89, 230, 181, 54, 82, 74, 42
    };
    public static long toInteger(BitSet key){
            long result=0;
            for(int i=0; i<key.length();i++)
                if(key.get(i)==true)
                    result+=pow(2,i);
            
            return result;
        }
        
        public static BitSet toBitset(long k,int size){
            BitSet temp=new BitSet(size);
            for(int j=0; j<size; j++){
                int t=(int)(k&1);
                boolean value=true;
                if(t==0)
                    value=false;
                temp.set(j, value);
        	k = k >> 1;                    
            }
            return temp;
        }
    
    public static final class Keys{
        public BitSet[] subKeys = new BitSet[16];
        
        /*
        Алгоритм генерации подключей:
        k[i]=(S^(i-1)[key],S^i[key]); i=1...16
        где S^i=S[S[S[...]]] (i раз)
        S=(s_box[key[i]]), i=0..7; key=(key[0],...,key[7])
        */
        
        Keys(BitSet key){
            BitSet newKey=(BitSet) key.clone();
            for(int i=0;i<16; i++){
                subKeys[i]=new BitSet(128);
                for(int j=0; j<64; j++)
                    subKeys[i].set(j,newKey.get(j));
                                
                for(int k=0; k<8; k++)
                {
                    long tempInt=toInteger(newKey.get(k*8,(k+1)*8));
                    tempInt=s_box[(short)tempInt];
                    BitSet tmpKey=(BitSet)toBitset(tempInt,8).clone();
                    
                    for(int l=0; l<8; l++)
                        subKeys[i].set(64+8*k+l,tmpKey.get(l));
                }
                newKey=(BitSet)subKeys[i].get(64,128).clone();
            }
        }
    }
    
    //Функция перестановки байтов внутри блока
    public static BitSet BP(BitSet text){
        BitSet result=new BitSet(128);
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                for(int k=0; k<8; k++){
                result.set(i*32+j*8+k,text.get(((i+j)*32+j*8+k)%128));
            }
        return result;
    }
    
    //Функция обратной перестановки байтов внутри блока
    public static BitSet BP_1(BitSet text){
        BitSet result=new BitSet(128);
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                for(int k=0; k<8; k++){                    
                result.set(((i+j)*32+j*8+k)%128,text.get(i*32+j*8+k));
            }
        return result;
    }
    
    //Функция начальной обработки
    public static BitSet IT(BitSet text, BitSet key1, BitSet key2){
                
        text.xor(key1);
        
        for(int i=0; i<4; i++){
            long Xi=toInteger(text.get(i*32,(i+1)*32));
            BitSet partOf_k14=(BitSet)key2.get(i*32,(i+1)*32).clone();
            partOf_k14.or(toBitset(1,32));
            long Bi=toInteger(partOf_k14);
            Xi*=Bi;
            Xi=(long) (Xi%(long)pow(2,32));
            BitSet Y=(BitSet)toBitset(Xi,32).clone();
            for(int j=0; j<32; j++)
                text.set(i*32 +j,Y.get(j));
        }
        
                
        return BP(text);
    }
    
    public static long solveCompare(long a, long b, long m){
        long u0,u1;
        long t1=a, t2=m;
        if(t1>t2){
            u0=0;
            u1=1;
        }
        else{
            u0=1;
            u1=0;
        }
        long u=1;
        long r=t1%t2;
        long q=t1/t2;
        while(r!=0){
            u=u0-q*u1;
            u0=u1;
            u1=u;
            t1=t2;
            t2=r;
            r=t1%t2;
            q=t1/t2;
        }
        return (u*b)%m;
    }
   
    //Функция завершающего преобразования
    public static BitSet FT(BitSet text, BitSet key1, BitSet key2){
        BitSet result;
        
        result=BP_1(text);
        
        for(int i=0; i<4; i++){
            long Yi=toInteger(result.get(i*32,(i+1)*32));
            BitSet partOf_k15=(BitSet)key2.get(i*32,(i+1)*32).clone();
            partOf_k15.or(toBitset(1,32));
            long Bi=toInteger(partOf_k15);
            
            long mod=(long)pow(2,32);
            long X=solveCompare(Bi,Yi,mod);
            
            BitSet Xi=(BitSet)toBitset(X,32).clone();
            for(int j=0; j<32; j++)
                result.set(i*32 +j,Xi.get(j));
        } 

        result.xor(key1);
        return result;
    }
    
    //Раундовая функция сети Фейстеля
    public static BitSet F(BitSet text,BitSet key, int round){
        BitSet result=new BitSet(128);
        BitSet[] rightPart=new BitSet[8];
        BitSet firstKey=(BitSet)key.get(0,64).clone();
        BitSet secondKey=(BitSet)key.get(64,128).clone();
         
        for(int i=0; i<8; i++)
        {
            rightPart[i]=new BitSet(8);
            rightPart[i]=(BitSet)text.get(64+i*8,64+(i+1)*8).clone();
        }
            
        //First XOR with 1st part of Key
        for(int i=0; i<8; i++)
            rightPart[i].xor(firstKey.get(i*8, (i+1)*8));
        //S-function
        for(int i=0; i<8; i++)
        {
            long tmp=(long)(s_box[(short)toInteger(rightPart[i])]);
            BitSet tempBitSet=(BitSet)toBitset(tmp,8).clone();
            for(int j=0; j<8; j++)
                rightPart[i].set(j,tempBitSet.get(j));
        }
        //XORs
        
        for(int i=7; i>3; i--)
            rightPart[i].xor(rightPart[i-4]);
        
        rightPart[3].xor(rightPart[5]);
        rightPart[2].xor(rightPart[4]);
        rightPart[1].xor(rightPart[7]);
        rightPart[0].xor(rightPart[6]);
        rightPart[7].xor(rightPart[2]);
        rightPart[6].xor(rightPart[1]);
        rightPart[5].xor(rightPart[0]);
        rightPart[4].xor(rightPart[3]);
        
        for(int i=3; i>-1; i--)
            rightPart[i].xor(rightPart[i+4]);
        

        //Second XOR with 2nd part of Key
        for(int i=0; i<8; i++)
            rightPart[i].xor(secondKey.get(i*8, (i+1)*8));
        
        //S-function
        for(int i=0; i<8; i++)
        {
            long tmp=(long)(s_box[(short)toInteger(rightPart[i])]);
            BitSet tempBitSet=(BitSet)toBitset(tmp,8).clone();
            for(int j=0; j<8; j++)
                rightPart[i].set(j,tempBitSet.get(j));
        }
        
        //BRL-function(циклический сдвиг влево)
        BitSet temp=(BitSet)rightPart[0].clone();
        for(int i=0; i<7; i++)
            rightPart[i]=(BitSet)rightPart[i+1].clone();
        rightPart[7]=(BitSet)temp.clone();
        
        //Формируем блок для следующего раунда сети Фейстеля
        BitSet R=new BitSet(64);
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                R.set(i*8+j,rightPart[i].get(j));
        
        BitSet L=(BitSet)text.get(0,64).clone();
        L.xor(R);
        
        //Если последний раунд, не нужно половинки блоков менять местами
        if(round<12)
            for(int i=0; i<64; i++){
                result.set(i,text.get(64+i));
                result.set(64+i,L.get(i));
            }
        else{
            for(int i=0; i<64; i++){
                result.set(i,L.get(i));
                result.set(64+i,text.get(64+i));
            }
        }
        return result;
    }

    
    public static BitSet Encryption(BitSet text, Keys keys){
        
        BitSet result=new BitSet(text.length());
       
            int border=(text.size()%128==0) ? (border=text.size()/128) :(border=text.size()/128 +1);
            for(int i=0; i<border; i++){
                BitSet block=(BitSet)text.get(i*128,(i+1)*128).clone();
                
                block=IT(block,keys.subKeys[12],keys.subKeys[13]);
                
                for(int k=0; k<12; k++)
                    block=F(block,keys.subKeys[k],k+1);
                
                block=FT(block,keys.subKeys[15],keys.subKeys[14]);
                
                for(int j=0; j<128; j++)
                    result.set(i*128+j,block.get(j));
            }
        return result;
    }
    
    public static BitSet Decryption(BitSet text,Keys keys){
        
        BitSet result=new BitSet(text.length());
        
            int border=(text.size()%128==0) ? (border=text.size()/128) :(border=text.size()/128 +1);
            for(int i=0; i<border; i++){
                BitSet block=(BitSet)text.get(i*128,(i+1)*128).clone();
                
                block=IT(block,keys.subKeys[15],keys.subKeys[14]);
                
                for(int k=0; k<12; k++)
                    block=F(block,keys.subKeys[11-k],k+1);
                
                block=FT(block,keys.subKeys[12],keys.subKeys[13]);
                
                for(int j=0; j<128; j++)
                    result.set(i*128+j,block.get(j));
            }
        return result;
    }
    
    public static void main(String[] args) {
        
        try {
            //Чтение исходного текста
            System.out.println("Введите путь к файлу для шифрования");
            Scanner sc = new Scanner(System.in);
            String s;
            s = sc.nextLine(); 
            String fileFormat=s.substring(s.lastIndexOf('.'), s.length());
            BitSet painText = BitSet.valueOf(Files.readAllBytes(Paths.get(s)));
           
            System.out.println("Введите путь к файлу с ключом");
            s = sc.nextLine();
            BitSet key = BitSet.valueOf(Files.readAllBytes(Paths.get(s)));
            
            //Генерация ключей, берём первые 64 бита, т.к. входные данные для функции генерации подключей имеют размер 64 бита
            int size=key.size();
            if(size<64)
                while(size<64){
                    for(int i=0; i<size; i++)
                        key.set(size+i,key.get(i));
                    
                    size=key.size();
                }
            Keys keys=new Keys(key.get(0, 64));
            
            //Шифрование текста
            BitSet encrypted=Encryption(painText,keys);

            //Запись в файл зашифрованного текста
            byte[] buf=new byte[encrypted.size()/8];
            System.arraycopy(encrypted.toByteArray(),0,buf,0,encrypted.toByteArray().length);
            Files.write(Paths.get("encrypted"+fileFormat),buf,StandardOpenOption.CREATE);
            
            //Чтение зашифрованного текста
            BitSet crypText = BitSet.valueOf(Files.readAllBytes(Paths.get("encrypted"+fileFormat)));
            
            //Расшифрование текста
            BitSet decrypted=Decryption(crypText,keys); 
   
            //Запись в файл расшифрованного текста
            byte[] buf1=new byte[decrypted.size()/8];
            System.arraycopy(decrypted.toByteArray(),0,buf1,0,decrypted.toByteArray().length);
            Files.write(Paths.get("decrypted"+fileFormat),buf1,StandardOpenOption.CREATE);
 
        } catch (IOException ex) {
            System.err.println(ex.getMessage());
        }
    }
}
