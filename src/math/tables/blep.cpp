
#include "blep.h"


namespace pdsp{

    inline_f void normalizeBufferPrecise(float* buffer, int len)
    {
        float max = 1E-37f;

        for (int i=0; i<len; ++i) {
            if (buffer[i] > max) {
                max = buffer[i];
            }
        }

        for (int i=0; i<len; ++i) {
            buffer[i] = buffer[i] / max;
        }
    }


    inline_f float* blepTableTemplate(const Window_t windowType, const int len, double pi_multiply)
    {

        float* table =nullptr;
        ofx_allocate_aligned(table, len+1);

        float* win = window(windowType, len);

        double onedivN_1 = 1.0 / static_cast<double>(len-1);  //-2 as we use N-1 and the buffersize is one sample less for being simmetrical

        table[0] = 0.0f;
        table[len] = 0.0f;  //guard point

        //TABLE GENERATION
        for (int i=0; i<len; ++i) {
            //double x = M_TAU_DOUBLE * static_cast<double>(points)*( (static_cast<double>(i) * onedivN_1) - 0.5);
            double x = M_PI_DOUBLE * pi_multiply * 2.0 * ((static_cast<double>(i) * onedivN_1) - 0.5 );  // 2 points if pi_multiply is 1, 8 if is 2, etc

            if(x!=0){
                table[i] = (sin(x)/x);
            }else{
                table[i] = 0.0f; //DISCONTINUITY
                //Logger::writeToLog("got discontinuity at index" + String(i+1));
            }
        }



        //WINDOWING
        for (int i=0; i<len; ++i) {
            table[i] = table[i] * win[i];
        }
        ofx_deallocate_aligned(win);//win is no more needed





        /* REVERSE EULER
        for (int i = 1; i < len; ++i) {
            table[i] = table[i] + table[i-1];
        }

        table[len] = table[len-1]; //guard point
        */

        // BILINEAR INTEGRATION
        float xn_z1 = 0.0f;
        float yn_z1 = 0.0f;

        for (int i = 0; i < len; ++i) {
            float temp = table[i];
            table[i] = yn_z1 + 0.5f * ( table[i] + xn_z1);
            //         y(n-1)+ T/2 * (  x(n)   + x(n-1) ) ;
            yn_z1 = table[i];
            xn_z1 = temp;
        }


        table[len] = table[len-1]; //guard point

        //NORMALIZE
        //normalize once
        normalizeBufferPrecise(table, len+1);
        //force normalization to get last value to 1.0f
        float last = table[len-1]; //len is the same
        for (int i=0; i<len; ++i) {
            table[i] = table[i] / last;
        }


        //CONVERT FROM UNIPOLAR TO BIPOLAR

        for (int i=0; i<len; ++i) {
            table[i] = (table[i] * 2.0f) - 1.0f;
        }


        //UNIT STEP SUBTRACTION
        for (int i=0; i<len; ++i) {
            if (table[i]>0) {
                table[i] -= 1.0f;
            }else{
                table[i] += 1.0f;
            }
        }

        /*checker

        table[len] = 0.0f;
        String info = "";
        usleep(200);
        int min = 2048-256;
        int max = min + 512;
        for(int i=min; i<max; ++i){
            info << " | ";
            info << String(table[i]);

            if( i==2047){
                info<<"| ------- INDEX 2047 ------ |";

            }


        }
        Logger::writeToLog(info);
        */
        return table;


    }


    float* blepTable2( const Window_t windowType, const int len)
    {
        float* table = blepTableTemplate(windowType, len, 1.0);
        return table;
    }

    float* blepTable8( const Window_t windowType, const int len)
    {
        float* table = blepTableTemplate(windowType, len, 2.0);
        return table;
    }


}
