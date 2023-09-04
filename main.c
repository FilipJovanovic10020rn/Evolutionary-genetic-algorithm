#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double x_r[50] = {
    0.00, 0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90,
    1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90,
    2.00, 2.10, 2.20, 2.30, 2.40, 2.50, 2.60, 2.70, 2.80, 2.90,
    3.00, 3.10, 3.20, 3.30, 3.40, 3.50, 3.60, 3.70, 3.80, 3.90,
    4.00, 4.10, 4.20, 4.30, 4.40, 4.50, 4.60, 4.70, 4.80, 4.90
};

double y_g[50] = {
    1.0000, 2.2667, 3.5533, 4.8599, 6.1865, 7.5332, 8.8998, 10.2865, 11.6931, 13.1197,
    14.5664, 16.0330, 17.5196, 19.0263, 20.5529, 22.0996, 23.6662, 25.2528, 26.8595, 28.4861,
    30.1327, 31.7994, 33.4860, 35.1927, 36.9193, 38.6659, 40.4326, 42.2192, 44.0258, 45.8525,
    47.6991, 49.5657, 51.4524, 53.3590, 55.2857, 57.2323, 59.1989, 61.1856, 63.1922, 65.2188,
    67.2655, 69.3321, 71.4188, 73.5254, 75.6520, 77.7987, 79.9653, 82.1519, 84.3586, 86.5852
};

// one neuron
double calc_neuron(int neurons, double input_weights[], double input_values[], double bias) {
	int i, j;
	double sum = bias;
	for (i = 0; i < neurons; i++) {
		sum += input_weights[i] * input_values[i];
	}
	return sum;
}

// calc net

#define L1N 4
#define L2N 4

double layer1[L1N][1];
double layer2[L2N][L1N];
double layer3[1][L2N];

double bias1[L1N];
double bias2[L2N];
double bias3[1];

double output0[1];
double output1[L1N];
double output2[L2N];

float izracunaj(int argc, double argv[]) {
    double *y_r = y_g;

    //char argv[34];
    //int argc=34;
    //for(int i=0;i<34;i++){
    //    argv[i]=1;
    //} domaci.exe 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33

    int total_expected_args = 1 + L1N + L1N * L2N + L2N + L1N + L2N + 1;//34
    if (argc != total_expected_args) {
        // broj argumenata nije odgovarajuci
        fprintf(stderr, "Broj argumenata nije odgovarajuci, ocekivano %d realnih vrednosti.\n", total_expected_args - 1);
        exit(1);
    }
    // parsiranje
    int i, j, k, ai = 1;
    // layer1
    for (i = 0; i < L1N; i++, ai++) {
        layer1[i][0] = argv[ai];
    }
    // layer2
    for (i = 0; i < L2N; i++) {
        for (j = 0; j < L1N; j++, ai++) {
            layer2[i][j] = argv[ai];
        }
    }
    // layer3
    for (i = 0; i < L2N; i++) {
        layer3[0][i] = argv[ai];
    }
    // bias1
    for (i = 0; i < L1N; i++, ai++) {
        bias1[i] = argv[ai];
    }
    // bias2
    for (i = 0; i < L2N; i++, ai++) {
        bias2[i] = argv[ai];
    }
    // bias3
    bias3[0] = argv[ai];

    // idemo dalje!
    double mse = 0;

    for (k = 0; k < 50; k++) {
        output0[0] = x_r[k];
        // layer1
        for (i = 0; i < L1N; i++) {
            output1[i] = calc_neuron(1, layer1[i], output0, bias1[i]);
        }
        // layer2
        for (i = 0; i < L2N; i++) {
            output2[i] = calc_neuron(L1N, layer2[i], output1, bias2[i]);
        }
        // layer3
        double val = calc_neuron(L2N, layer3[0], output2, bias3[0]);

        double err = pow(y_r[k] - val, 2);
        mse += err;
    }
    float resenje;
    //printf("\n Trosak je %.5f\n", mse / 50);
    resenje = (mse*1.0)/50;
    return resenje;
}

void ukrstanje(double hromozom1[],double hromozom2[],double hromozom3[]){

    float random;

    for(int i =0;i<33;i++){
        random = ((float)rand()/(float)(RAND_MAX)) *1.0;
        //printf("random je %f stara %f ",random,hromozom3[i]);
        hromozom3[i] = random*(hromozom1[i]-hromozom2[i]) + hromozom1[i];
        if(hromozom3[i]>3.0)
            hromozom3[i] = 3.0;
        else if(hromozom3[i]<3.0*(-1))
            hromozom3[i] = 3.0*(-1);
        //printf("nova %f\n",hromozom3[i]);
    }
    //printf("\n");
}
void mutacija(double hromozom1[]){
    int random = rand()%33;
    //printf("%d Stari gen je %f\n",random,hromozom1[random]);
    hromozom1[random] = (((float)rand()/(float)(RAND_MAX)) *6.0)-3;
    //printf("Nov gen je %f\n",hromozom1[random]);

}

int main(){

    int populacija =100;
    int brojPokretanja = 150;
    int brojJedinkiZaParenje = 80;
    int brojJedinkiZaMutaciju = 40;
    int brojJedinkiZaIzbacivanje = brojJedinkiZaParenje-1;
    float sansaMutacije = 0.3;

    int izabraniZaParenje[100];

    int generacija=2;
/*
    float hromozom[100][33];
    float a = 6.0;
    float gen=1;
    for(int i =0; i<=33;i++){
        gen=((float)rand()/(float)(RAND_MAX)) * a;
        gen-=3;
        printf("%f\n", gen);
    }*/
    int flagM=0;
    int flagU=0;



    int promenljiva;
    float promenljivaSanse;

    double pocetniHromozom[100][34];
    double hromozom[100][34];
    double deteHromozom[100][34];
    double hromozomiNoveGeneracije[100][34];

    double suma=0;
    double srenjaVrednostPoGeneraciji[151];
    int najboljiHromozomPoGeneraciji[151];
    double najboljiHromozom[151][34];


    double srenjaVrednostPoGeneraciji1[151];
    int najboljiHromozomPoGeneraciji1[151];
    double najboljiHromozom1[151][34];


     double srenjaVrednostPoGeneraciji2[151];
     int najboljiHromozomPoGeneraciji2[151];
    double najboljiHromozom2[151][34];

    int first=0;

    int koefNajboljeg=-1;
    double minimum = 9999999999999999999.0;
    double najnaj[100][34];

    float resenja[100];
    float resenjaSortiranja[100];
    int resenjaRedosled[100];

    float resenjaDece[100];
    float resenjaSortiranjaDece[100];
    int resenjaRedosledDece[100];

    int operation = 0;

    //char output[50];
    printf("1 - Pokreni algoritam\n2 - Promeni broj populacije\n3 - Promeni broj generacija\n4 - Promeni broj dece\n5 - Promeni broj jedinki za mutaciju\n6 - Promeni broj uzastopnih ponavljanja\n7 - Promeni sansu za mutaciju\n8 - Prikazi trenutne parametre\n9 - Pomoc\n10 - Ispisi srednje vrednosti generacija\n0 - Izlaz\nPozeljno je ne unositi slova jer ce puci\n\n");
    while(1){
        promenljiva=0;
        promenljivaSanse = 0;
        scanf("%d",&operation);
         switch(operation)
        {
            case 1:

                printf("Pokrece se algoritam\n");
                    double a = 6.0;
                    double gen;
                    first=1;
                    for(int j=0;j<populacija;j++){
                        for(int i =0; i<33;i++){
                            gen=((float)rand()/(float)(RAND_MAX)) * a;
                            gen-=3;
                            hromozom[j][i]=gen;
                            pocetniHromozom[j][i]=gen;
                        }
                    }
                //printf("%d Hromozoma je generisano",populacija);
//****************************************************************************************************

                for(int z=0;z<generacija;z++){
                        printf("%d. PONAVLJANE \n\{\n",z);
                        minimum = 9999999999999999999.0;
                        for(int j=0;j<populacija;j++){
                            for(int i=0;i<33;i++){
                                hromozom[j][i]=pocetniHromozom[j][i];
                            }
                        }
                for(int k = 0;k<brojPokretanja;k++){
                    //printf("Soritranje");
                    for(int i = 0;i<populacija;i++){
                        resenja[i]=izracunaj(34,hromozom[i]);
                        resenjaSortiranja[i]=resenja[i];
                    }
                    float temp=0;
                    for(int i= 0; i<populacija-1;i++){
                            for(int j=i+1;j<populacija;j++){
                                if(resenjaSortiranja[i]>resenjaSortiranja[j]){
                                    temp =resenjaSortiranja[i];
                                    resenjaSortiranja[i]=resenjaSortiranja[j];
                                    resenjaSortiranja[j]=temp;
                                }
                            }
                    }
                    //printf("\n");
                    /*for(int i = 0;i<populacija;i++){
                        printf("%f\n",resenjaSortiranja[i]);
                    }*/
                    int brojac=0;
                    for(int i =0;i<populacija;i++){
                        for(int j=0;j<populacija;j++){
                            if(resenjaSortiranja[i] == resenja[j]){
                                resenjaRedosled[brojac++]=j;
                            }
                        }
                    }
                    //printf("Sortirani su hromozomi po najboljim vrednostima\n");
                    for(int i = 0;i<populacija;i++){
                        //printf("%d   %f\n",resenjaRedosled[i],resenja[resenjaRedosled[i]]);
                    }
                    //printf("Najbolji je %d sa troskom %f\n",resenjaRedosled[0],resenja[resenjaRedosled[0]]);

                   // for(int i =0;i<brojJedinkiZaParenje;i++){
                   //     izabraniZaParenje[i]=resenjaRedosled[i];
                   //     printf("%f\n",resenja[izabraniZaParenje[i]]);
                    //}
                   // printf("a izbacuje se %f\n",resenja[resenjaRedosled[populacija-1]]);

//****************************************************************************************************
                    //printf("PARENJE\n");

                    //selekcija
                    for(int k=0;k<brojJedinkiZaParenje;k++){
                        //printf("SELEKCIJA\n");
                        int selektovan1=rand()%populacija;
                        int selektovan2=-1;
                        for(int i = 0;i<populacija;i++){
                            selektovan2=rand()%populacija;
                            if(selektovan1!=selektovan2){
                                break;
                            }
                        }
                        if(selektovan1==selektovan2 || selektovan2==-1){
                            selektovan2=(selektovan1+1)%populacija;
                        }
                        //printf("Selekcija uspesna\n");
                        //printf("UKRSTRANJE\n");
                        ukrstanje(hromozom[selektovan1],hromozom[selektovan2],deteHromozom[k]);
                        //printf("Ukrstanje uspesno\n");

                        /*
                        for(int i =0;i<brojJedinkiZaParenje-1;i++){
                            ukrstanje(hromozom[izabraniZaParenje[i]],hromozom[izabraniZaParenje[i+1]],hromozom[resenjaRedosled[populacija-1]]);
                        }
                        printf("Nov trosak je %f\n",izracunaj(34,hromozom[resenjaRedosled[populacija-1]]));
                        */

                    }

//****************************************************************************************************
//ZAMENI HROMOZOM SA HROMOZOMDECA
                    //printf("MUTACIJA\n");
                    int mutirani[100];
                    for(int i =0;i<100;i++){
                        mutirani[i]=0;
                    }
                    int brojacZaMutaciju =0;
                    for(int i =0;i<brojJedinkiZaMutaciju;i++){
                            //printf("%d",brojacZaMutaciju);
                        for(int j=0;j<brojJedinkiZaParenje;j++){
                            float randomSansa = ((float)rand()/(float)(RAND_MAX)) * 1.0;
                            int random = rand()%brojJedinkiZaParenje;
                            //printf("sanse je %f\n",randomSansa);
                            if(mutirani[random]==0){
                                if((randomSansa<=sansaMutacije)){
                                    mutirani[random]=1;
                                    //printf("\n Mutira se hromozom sa trsokom %d, %f\n",random,resenja[random]);
                                    mutacija(deteHromozom[random]);
                                    //printf("\nNov trosak je %f\n",izracunaj(34,hromozom[random]));
                                    break;
                                }
                            }
                            else{
                                //printf("%d je vec mutiran\n",random);
                            }
                            if(j==brojJedinkiZaParenje-1 && mutirani[j]==0){
                                //printf("Nije se desila mutacija\n");
                                brojacZaMutaciju++;
                            }
                        }
                    }
                    //printf("Nije se desila %d mutacija od namenjenih %d\n",brojacZaMutaciju,brojJedinkiZaMutaciju);

/*

                    for(int i = 0;i<brojJedinkiZaMutaciju;i++){
                            printf("%d mutacija\n",i);
                        for(int j =0;j<populacija;j++){
                            float randomSansa = ((float)rand()/(float)(RAND_MAX)) * 1.0;
                            int random = rand()%populacija;
                            printf("sanse je %f\n",randomSansa);
                            if(mutirani[random]==0){
                                if((randomSansa<=sansaMutacije)){
                                    mutirani[random]=1;
                                    printf("\n Mutira se hromozom sa trsokom %d, %f\n",random,resenja[random]);
                                    mutacija(hromozom[random]);
                                    printf("\nNov trosak je %f\n",izracunaj(34,hromozom[random]));
                                    break;
                                }
                            }
                            else{
                                printf("%d je vec mutiran\n",random);
                            }
                            if(j==populacija-1 && mutirani[j]==0){
                                //printf("Nije se desila mutacija\n");
                                brojacZaMutaciju++;
                            }
                        }
                    }
                    printf("Nije se desila %d mutacija od namenjenih %d",brojacZaMutaciju,brojJedinkiZaMutaciju);
*/

//****************************************************************************************************

                    //printf("Sortiranje 2");

                    for(int i = 0;i<brojJedinkiZaParenje;i++){
                        resenjaDece[i]=izracunaj(34,deteHromozom[i]);
                        resenjaSortiranjaDece[i]=resenjaDece[i];
                    }
                float temp3=0;
                    for(int i= 0; i<brojJedinkiZaParenje-1;i++){
                            for(int j=i+1;j<brojJedinkiZaParenje;j++){
                                if(resenjaSortiranjaDece[i]>resenjaSortiranjaDece[j]){
                                    temp3 =resenjaSortiranjaDece[i];
                                    resenjaSortiranjaDece[i]=resenjaSortiranjaDece[j];
                                    resenjaSortiranjaDece[j]=temp3;
                                }
                            }
                    }
                    //printf("\n");
                    /*for(int i = 0;i<populacija;i++){
                        printf("%f\n",resenjaSortiranja[i]);
                    }*/
                    int brojac3=0;
                    for(int i =0;i<brojJedinkiZaParenje;i++){
                        for(int j=0;j<brojJedinkiZaParenje;j++){
                            if(resenjaSortiranjaDece[i] == resenjaDece[j]){
                                resenjaRedosledDece[brojac3++]=j;
                            }
                        }
                    }
                    //printf("Sortirani su hromozomi po najboljim vrednostima\n");
                    for(int i = 0;i<brojJedinkiZaParenje;i++){
                        //printf("%d   %f\n",resenjaRedosledDece[i],resenjaDece[resenjaRedosledDece[i]]);
                    }
                    //printf("Najbolji je %d sa troskom %f\n",resenjaRedosledDece[0],resenjaDece[resenjaRedosledDece[0]]);


//****************************************************************************************************
                //printf("Formiranje nove populacije sa decom");
                    int brojacRoditelja=0;
                    int provera=0;
                    //ODABIR RODITELJA KOJI IDU DALJE
                    for(int i=0;i<populacija*2/10;i++){
                            for(int j=0;j<brojJedinkiZaParenje-brojacRoditelja;j++){
                                if(j==populacija*2/10){
                                    provera=1;
                                    break;
                                }
                                if(resenjaDece[resenjaRedosledDece[j]]>resenja[resenjaRedosled[i]]){
                                    brojacRoditelja++;
                                    break;
                                }
                            }
                        if(provera){
                            break;
                        }
                    }
                    int brojacRoditelja2=0;
                    int brojacZaDecu=0;
                    for(int i=0;i<populacija;i++){
                        if(brojacRoditelja<brojacRoditelja2){

                            for(int j=0;j<33;j++){
                                hromozomiNoveGeneracije[i][j]=hromozom[brojacRoditelja2][j];
                            }
                            brojacRoditelja2++;
                        }
                        else{
                            if(brojacZaDecu!=brojJedinkiZaParenje){

                                for(int j=0;j<33;j++){
                                    hromozomiNoveGeneracije[i][j]=deteHromozom[brojacZaDecu][j];
                                }

                                brojacZaDecu++;
                            }
                            else{
                                for(int j=0;j<33;j++){
                                    hromozomiNoveGeneracije[i][j]=hromozom[brojacRoditelja2][j];
                                }
                                brojacRoditelja2++;
                            }
                        }
                    }

                    for(int i=0;i<populacija;i++){
                        for(int j=0;j<33;j++){
                            hromozom[i][j]=hromozomiNoveGeneracije[i][j];
                        }
                    }

//****************************************************************************************************
                //printf("kalkulacija konvergencije i cuvanje podataka");

                    //printf("Sortiranje3");
                if(z==0){
                    for(int i = 0;i<populacija;i++){
                        resenja[i]=izracunaj(34,hromozom[i]);
                        resenjaSortiranja[i]=resenja[i];
                    }
                    float temp5=0;
                    for(int i= 0; i<populacija-1;i++){
                            for(int j=i+1;j<populacija;j++){
                                if(resenjaSortiranja[i]>resenjaSortiranja[j]){
                                    temp5 =resenjaSortiranja[i];
                                    resenjaSortiranja[i]=resenjaSortiranja[j];
                                    resenjaSortiranja[j]=temp5;
                                }
                            }
                    }
                    //printf("\n");
                    /*for(int i = 0;i<populacija;i++){
                        printf("%f\n",resenjaSortiranja[i]);
                    }*/
                    int brojac9=0;
                    for(int i =0;i<populacija;i++){
                        for(int j=0;j<populacija;j++){
                            if(resenjaSortiranja[i] == resenja[j]){
                                resenjaRedosled[brojac9++]=j;
                            }
                        }
                    }
                    //printf("Sortirani su hromozomi po najboljim vrednostima\n");
                    for(int i = 0;i<populacija;i++){
                        //printf("%d   %f\n",resenjaRedosled[i],resenja[resenjaRedosled[i]]);
                    }
                    //printf("Najbolji je %d sa troskom %f\n",resenjaRedosled[0],resenja[resenjaRedosled[0]]);

                    suma=0;
                    for(int i =0;i<populacija;i++){
                        suma+=resenja[i];
                    }
                    srenjaVrednostPoGeneraciji[k]=suma/populacija;
                    najboljiHromozomPoGeneraciji[k]=resenjaRedosled[0];
                    for(int i =0;i<33;i++){
                        najboljiHromozom[k][i]= hromozom[resenjaRedosled[0]][i];
                    }

                    printf("Srednja vrednost %d. generacije je %f, a najbolji hromozom je %d sa troskom %f\n",k,srenjaVrednostPoGeneraciji[k],najboljiHromozomPoGeneraciji[k],izracunaj(34,najboljiHromozom[k]));
                    //printf("%f\n",resenja[resenjaRedosled[0]]);

                    if(izracunaj(34,najboljiHromozom[k]) < minimum){
                        minimum=izracunaj(34,najboljiHromozom[k]);
                        koefNajboljeg=k;
                        for(int p=0;p<33;p++){
                            najnaj[z][p]=najboljiHromozom[koefNajboljeg][p];
                        }
                    }
                }
                if(z==1){
                    for(int i = 0;i<populacija;i++){
                        resenja[i]=izracunaj(34,hromozom[i]);
                        resenjaSortiranja[i]=resenja[i];
                    }
                    float temp5=0;
                    for(int i= 0; i<populacija-1;i++){
                            for(int j=i+1;j<populacija;j++){
                                if(resenjaSortiranja[i]>resenjaSortiranja[j]){
                                    temp5 =resenjaSortiranja[i];
                                    resenjaSortiranja[i]=resenjaSortiranja[j];
                                    resenjaSortiranja[j]=temp5;
                                }
                            }
                    }
                    //printf("\n");
                    /*for(int i = 0;i<populacija;i++){
                        printf("%f\n",resenjaSortiranja[i]);
                    }*/
                    int brojac9=0;
                    for(int i =0;i<populacija;i++){
                        for(int j=0;j<populacija;j++){
                            if(resenjaSortiranja[i] == resenja[j]){
                                resenjaRedosled[brojac9++]=j;
                            }
                        }
                    }
                    //printf("Sortirani su hromozomi po najboljim vrednostima\n");
                    for(int i = 0;i<populacija;i++){
                        //printf("%d   %f\n",resenjaRedosled[i],resenja[resenjaRedosled[i]]);
                    }
                    //printf("Najbolji je %d sa troskom %f\n",resenjaRedosled[0],resenja[resenjaRedosled[0]]);

                    suma=0;
                    for(int i =0;i<populacija;i++){
                        suma+=resenja[i];
                    }
                    srenjaVrednostPoGeneraciji1[k]=suma/populacija;
                    najboljiHromozomPoGeneraciji1[k]=resenjaRedosled[0];
                    for(int i =0;i<33;i++){
                        najboljiHromozom1[k][i]= hromozom[resenjaRedosled[0]][i];
                    }

                    printf("Srednja vrednost %d. generacije je %f, a najbolji hromozom je %d sa troskom %f\n",k,srenjaVrednostPoGeneraciji1[k],najboljiHromozomPoGeneraciji1[k],izracunaj(34,najboljiHromozom1[k]));
                    //printf("%f\n",resenja[resenjaRedosled[0]]);

                    if(izracunaj(34,najboljiHromozom1[k]) < minimum){
                        minimum=izracunaj(34,najboljiHromozom1[k]);
                        koefNajboljeg=k;
                        for(int p=0;p<33;p++){
                            najnaj[z][p]=najboljiHromozom1[koefNajboljeg][p];
                        }
                    }
                }
                if(z==2){
                    for(int i = 0;i<populacija;i++){
                        resenja[i]=izracunaj(34,hromozom[i]);
                        resenjaSortiranja[i]=resenja[i];
                    }
                    float temp5=0;
                    for(int i= 0; i<populacija-1;i++){
                            for(int j=i+1;j<populacija;j++){
                                if(resenjaSortiranja[i]>resenjaSortiranja[j]){
                                    temp5 =resenjaSortiranja[i];
                                    resenjaSortiranja[i]=resenjaSortiranja[j];
                                    resenjaSortiranja[j]=temp5;
                                }
                            }
                    }
                    //printf("\n");
                    /*for(int i = 0;i<populacija;i++){
                        printf("%f\n",resenjaSortiranja[i]);
                    }*/
                    int brojac9=0;
                    for(int i =0;i<populacija;i++){
                        for(int j=0;j<populacija;j++){
                            if(resenjaSortiranja[i] == resenja[j]){
                                resenjaRedosled[brojac9++]=j;
                            }
                        }
                    }
                    //printf("Sortirani su hromozomi po najboljim vrednostima\n");
                    for(int i = 0;i<populacija;i++){
                        //printf("%d   %f\n",resenjaRedosled[i],resenja[resenjaRedosled[i]]);
                    }
                    //printf("Najbolji je %d sa troskom %f\n",resenjaRedosled[0],resenja[resenjaRedosled[0]]);
                    suma=0;
                    for(int i =0;i<populacija;i++){
                        suma+=resenja[i];
                    }
                    srenjaVrednostPoGeneraciji2[k]=suma/populacija;
                    najboljiHromozomPoGeneraciji2[k]=resenjaRedosled[0];
                    for(int i =0;i<33;i++){
                        najboljiHromozom2[k][i]= hromozom[resenjaRedosled[0]][i];
                    }

                    printf("Srednja vrednost %d. generacije je %f, a najbolji hromozom je %d sa troskom %f\n",k,srenjaVrednostPoGeneraciji2[k],najboljiHromozomPoGeneraciji2[k],izracunaj(34,najboljiHromozom2[k]));
                    //printf("%f\n",resenja[resenjaRedosled[0]]);

                    if(izracunaj(34,najboljiHromozom2[k]) < minimum){
                        minimum=izracunaj(34,najboljiHromozom2[k]);
                        koefNajboljeg=k;
                        for(int p=0;p<33;p++){
                            najnaj[z][p]=najboljiHromozom2[koefNajboljeg][p];
                        }
                    }
                }

                }
                printf("Trosak najboljeg hromozoma %d. ponavaljnja je %f\n",z,izracunaj(34,najnaj[z]));
                printf("Sastav najboljeg hromozoma %d. ponavaljnja je:\n",z);
                for(int i =0;i<33;i++){
                        if(i%11 == 0){
                            printf("\n");
                        }
                    printf("%f  ",najnaj[z][i]);
                }
                printf("\n\}\n\n");

                }
                //printf("\n");
//****************************************************************************************************
                break;
            case 2:
                printf("Unesi broj populacije ili 0 da bi se vratio nazad\n");
                while(1){
                    scanf("%d",&promenljiva);
                    if(promenljiva == 0){
                        break;
                    }
                    else if(promenljiva>100){
                        printf("Ne moze broj populacija biti veci od 100\n");
                    }
                    else if(promenljiva<10){
                        printf("Ne moze broj populacija biti manji od 10\n");
                    }
                    else{
                        populacija = promenljiva;
                        if(brojJedinkiZaMutaciju>populacija){
                            brojJedinkiZaMutaciju=populacija*4/10;
                            flagM=1;
                        }
                        if(brojJedinkiZaParenje>populacija){
                            brojJedinkiZaParenje=populacija*8/10;
                            flagU=1;
                        }

                        break;
                    }
                }
                if(promenljiva != 0)
                    printf("Populacija je sada %d\n",populacija);
                if(flagM){
                    flagM=0;
                    printf("Zato sto je populacija sada veca smanjuje se broj jedinki za mutaciju na %d\n",brojJedinkiZaMutaciju);
                }
                if(flagU){
                    flagU=0;
                    printf("Zato sto je populacija sada veca smanjuje se broj dece na %d\n",brojJedinkiZaParenje);
                }
                break;
            case 3:
                printf("Unesi broj generacija ili 0 da bi se vratio nazad\n");
                while(1){
                    scanf("%d",&promenljiva);
                    if(promenljiva == 0){
                        break;
                    }
                    else if(promenljiva>150){
                        printf("Ne moze broj generacija biti veci od 150\n");
                    }
                    else if(promenljiva<2){
                        printf("Ne moze broj generacija biti manji od 2\n");
                    }
                    else{
                        brojPokretanja= promenljiva;
                        break;
                    }
                }
                if(promenljiva != 0)
                    printf("Broj generacija je sada %d\n",brojPokretanja);
                break;
            case 4:
                printf("Unesi broj dece ili 0 da bi se vratio nazad\n");
                while(1){
                    scanf("%d",&promenljiva);
                    if(promenljiva == 0){
                        break;
                    }
                    else if(promenljiva>populacija){
                        printf("Ne moze broj dece biti veci od populacije\n");
                    }
                    else if(promenljiva< 2){
                        printf("Ne moze broj dece biti manji od 2\n");
                    }
                    else{
                        brojJedinkiZaParenje = promenljiva;
                        break;
                    }
                }
                if(promenljiva != 0)
                    printf("Broj dece je sada %d\n",brojJedinkiZaParenje);
                break;
            case 5:
                printf("Unesi broj jedinki za mutaciju ili 0 da bi se vratio nazad\n");
                while(1){
                    scanf("%d",&promenljiva);
                    if(promenljiva == 0){
                        break;
                    }
                    else if(promenljiva>populacija){
                        printf("Ne moze broj jedinki za mutaciju biti veci od populacije\n");
                    }
                    else if(promenljiva<1){
                        printf("Ne moze broj jedinki za mutaciju biti manji od 1\n");
                    }
                    else{
                        brojJedinkiZaMutaciju = promenljiva;
                        break;
                    }
                }
                if(promenljiva != 0)
                    printf("Broj jedinki za mutaciju je sada %d\n",brojJedinkiZaMutaciju);
                break;
            case 6:
                printf("Unesi broj uzastopnih povaljanja ili 0 da bi se vratio nazad\n");
                while(1){
                    scanf("%d",&promenljiva);
                    if(promenljiva == 0){
                        break;
                    }
                    else if(promenljiva>3){
                        printf("Ne moze broj uzastopnih ponavaljanja biti veci od 3\n");
                    }
                    else if(promenljiva<1){
                        printf("Ne moze broj uzastopnih ponvaljanja biti manji od 1\n");
                    }
                    else{
                        generacija = promenljiva;
                        break;
                    }
                }
                if(promenljiva != 0)
                    printf("Broj uzastopnih ponavljanja je sada %d\n",generacija);
                break;
            case 7:
                printf("Unesi broj sanse za mutaciju ili 0 da bi se vratio nazad\n");
                while(1){
                    scanf("%f",&promenljivaSanse);
                    if(promenljivaSanse == 0){
                        break;
                    }
                    else if(promenljivaSanse>1){
                        printf("Ne moze broj sanse za mutaciju biti veci od 1\n");
                        printf("Unesi broj sanse za mutaciju ili 0 da bi se vratio nazad\n");
                    }
                    else if(promenljivaSanse<0.3){
                        printf("Ne moze broj sanse za mutaciju biti manji od 0.3\n");
                        printf("Unesi broj sanse za mutaciju ili 0 da bi se vratio nazad\n");
                    }
                    else{
                        sansaMutacije = promenljivaSanse;
                        break;
                    }
                }
                if(promenljivaSanse != 0)
                    printf("Broj sanse za mutaciju je sada %f\n",sansaMutacije);
                break;
            case 8:
                printf("Populacija - %d\nBroj generacija - %d\nBroj dece - %d\nBroj jedinki za mutaciju - %d\nBroj uzastopnih ponavljanja - %d\nSansa mutacije - %f\n",populacija,brojPokretanja,brojJedinkiZaParenje,brojJedinkiZaMutaciju,generacija,sansaMutacije);
                break;
            case 9:
                printf("1 - Pokreni algoritam\n2 - Promeni broj populacije\n3 - Promeni broj generacije\n4 - Promeni broj dece\n5 - Promeni broj jedinki za mutaciju\n6 - Promeni broj uzastopnih ponavljanja\n7 - Promeni sansu za mutaciju\n8 - Prikazi trenutne parametre\n9 - Pomoc\n10 - Ispisi srednje vrednosti generacija\n0 - Izlaz\nPozeljno je ne unositi slova jer ce puci\n\n");
                break;
            case 10:
                if(first){
                    for(int z=0;z<generacija;z++){
                        printf("%d. PONAVLJANJE \n\{\n",z);
                        for(int i=0;i<brojPokretanja;i++){
                            printf("U %d. generaciji srednji trosak je %f sa najboljim hromozomom %d sa troskom %f\n",i,srenjaVrednostPoGeneraciji[i],najboljiHromozomPoGeneraciji[i],izracunaj(34,najboljiHromozom[i]));
                        }
                                        printf("Trosak najboljeg gromozoma je %f\n",izracunaj(34,najnaj[z]));
                        printf("Sastav najboljeg hromozoma je:\n");
                        for(int i =0;i<33;i++){
                                if(i%11 == 0){
                                    printf("\n");
                                }
                            printf("%f  ",najnaj[z][i]);
                        }
                        printf("\n\}\n");
                    }
                }
                else{
                    printf("Morate prvo pokrenuti algoritam\n");
                }
                break;
            case 0:
                return;
            default:
                    printf("Niste uneli dobar broj unesite 9 za pomoc\n");

        }
    }
    return 0;
}



