#include <stdio.h> //input-output
#include <string.h> //strenger
#include <stdint.h> //heltall med eksplisitt bredde
#include <stdlib.h> //diverse

#include "motordriver.h"


/**
 * Dette er litt tungvint, og mest for aa vise
 * funksjonspekere. I noen tilfeller, saerlig
 * hvis man jobber med mikrokontrollere, er
 * det derimot nyttig aa kunne lage enkle
 * funksjoner for aa haandtere strenger selv.
 * Da vil man ha noe saant som dette i bunnen.
 */
void print_string(char* stringStart)
{
    //Kopierer stringStart til en lokal variabel
    //for aa ikke risikere aa oedelegge strengen
    //utenfor funksjonen
    char* currentChar = stringStart;

    while (*currentChar)
    {
        printf("%c", *currentChar);
        currentChar++;
    }
    printf("\n");
    
}

/**
 * Dette er i praksis det samme som i eksempelet med uint32
 * og uint8 nedenfor, men her sier vi eksplisitt at 'bytes'
 * refererer til det samme minneomraadet som 'integer'.
 * Konseptet "register" er essensielt for mikrokontrollerprogrammering,
 * mer info kommer!
 */
typedef union
{
    int32_t integer;
    uint8_t bytes[4];
} Register_t;



int main(int argc, char* argv[])
{

    ///////////////////////////
    //Bruk av inkludert modul//
    ///////////////////////////

    motor_t motor_test = 
    {
        1,
        120,
        0,
        0,
        4
    };

    //Henter ut motorkraft og spenning via grensesnittet. 
    //Argumentet leses som "adressa til motor_test"
    int16_t power = motor_get_power(&motor_test);
    int16_t voltage = motor_get_voltage(&motor_test);
    printf("Kraft: %i W | Spenning: %i V\n", power, voltage);

    //Grensesnittet lar oss endre paa motoren uten aa maatte
    //tenke paa hva som skjer bak kulissene. Hvis vi ser at det
    //er noe galt med maaten vi manipulerer motor_test, trenger
    //vi BARE endre det i grensesnitt-funksjonene
    int16_t newVoltage = 30;
    motor_set_voltage(&motor_test, newVoltage);
    motor_update_power(&motor_test);
    //Det er generelt daarlig praksis aa noeste funksjonskall, men det funker!
    printf("Kraft: %i W | Spenning: %i V\n", motor_get_power(&motor_test), motor_get_voltage(&motor_test));

    
    
    //////////////////
    //Bruk av pekere//
    //////////////////

    //Helt grunnleggende:

    int32_t heltall = 345;
    int32_t* p_heltall = &heltall; //Note to self: demonstrer -Werror og -Wno-unused-variable her
    //Merker oss to ting:
    //Adresse er en 16-sifret (8 byte) verdi fordi jeg jobber paa et 64-bit minnesystem,
    //og *p_heltall blir faktisk satt til verdien av heltall
    printf("Adresse: %p | Verdi: %i\n", p_heltall, *p_heltall);
    printf("Pekerstoerrelse: %li \n", sizeof(p_heltall));


    //Tekststrenger er egentlig lister av bokstaver, og avsluttes med NULL:
    char tekststreng[] = "Hei paa deg din gamle sei\n";
    printf("Ren streng: %s", tekststreng);

    //Vi kan iterere gjennom lista, og stoppe naar vi kommer til NULL
    printf("Som liste: ");
    //"Adressa til tekststreng" er egentlig adressa til det foerste
    //elementet i lista
    char* currentChar = &tekststreng; //Note to self: forklar at man kan bruke tekststreng[0] for aa unngaa incomp.pt, skru paa -Wno-incomp.
    //Loopen kjoerer saa lenge den derefererte verdien til currentChar
    //ikke er NULL, altsaa 0x00. Dette er et eksempel paa at vi bruker
    //while naar vi ikke vet hvor mange iterasjoner vi trenger.
    while(*currentChar)
    {
        //Print ut den derefererte verdien og legg til 1 paa pekerverdien
        printf("%c", *currentChar);
        currentChar++;
    }
    printf("\n");

    
    //Lister kan like fint itereres gjennom baklengs, ved aa dekrementere pekeren:
    printf("Baklengs: ");
    //Har ikke noen NULL-karakter i starten, definerer derfor
    //start-addressa til tekststreng som 'stoppested'
    char* stopChar = &tekststreng;
    while (currentChar > stopChar)
    {
        //Husk at vi inkrementerte currentChar til slutten av strengen
        //i forrige loop, og dit peker den fortsatt!
        currentChar--;
        printf("%c", *currentChar);
    }
    printf("\n");
    

    //I C er alt data, som vi skal se her:

    printf("Uint32 per byte: ");
    //0xDEADBEEF er en mye brukt placeholder i embedded-verdenen,
    //et heksadesimalt tall som tilfeldigvis ogsaa kan leses som et ord
    uint32_t u32example = 0xDEADBEEF;
    //Naa spisser det seg til:
    //"u8-pekeren u32AsArray skal rettes mot adressa til u32example, som skal tolkes som en u8-adresse"
    //Casting til u8-peker er ikke strengt noedvendig, men unngaar at kompilatoren blir irritert
    uint8_t* u32AsArray = (uint8_t*)&u32example;

    //To hjelpevariabler, saa kommer vi til poenget:
    int bytes_in_u32 = 4;
    int offset = 0;
    //Vi kan endelig aksessere hver enkelt byte i u32example!
    while(offset < bytes_in_u32)
    {
        printf("%X", *(u32AsArray + offset));
        offset++;
    }
    printf("\n");
    //Outputen er forvirrende, men helt riktig. Dette viser oss at
    //minnesystemet i datamaskinen min er little endian, altsaa at
    //den siste byten i hver variabel blir skrevet foerst.
    //Endianness er en god grunn til at vi maa vaere forsiktige med
    //aa operere direkte paa minnet, og i stoerst mulig grad overlate
    //den slags til kompilatoren eller operativsystemet.
    
    //For ordens skyld kjoerer vi dette baklengs:
    printf("U32 per byte, baklengs: ");
    while (offset > 0) //Igjen: offset er fortsatt 4!
    {
        offset--;
        printf("%X", *(u32AsArray + offset));
    }
    printf("\n");


    //Selv om minnet er little endian, gjelder det bare for
    //grunnleggende datatyper som hel- og flyttall. Hvis
    //vi lagrer den samme sekvensen av bytes som en liste
    //av u8, kan vi hente det ut i "riktig" rekkefoelge.
    uint8_t byteArray[] = {0xDE, 0xAD, 0xBE, 0xEF};
    offset = 0;
    while (offset < 4)
    {
        //Her bruker vi forresten den vanlige syntaksen for a aksessere lister, listeNavn[indeks]
        //Vi kunne ogsaa skrevet *(&byteArray[0]+offset) for aa hente ut verdiene.
        printf("Adresse: %p | Verdi: %X\n", &byteArray[offset], byteArray[offset]);
        offset++;
    }


    ///////////////////////////////////////////////
    //Dobbeltpekere: Matriser, lister av strenger//
    ///////////////////////////////////////////////
    
    printf("\nMatriser: \n");
    //Foerste indeks er en peker til startadressa
    //til lista som utgjoer andre indeks. For
    //multidimensjonale lister er det kun siste
    //indeks som faktisk peker paa en verdi.
    uint8_t byteMatrix[3][4] =
    {
        {0xDE, 0xAD, 0xBE, 0xEF}, //byteMatrix[0] peker paa adressa til 0xDE
        {0x0D, 0x15, 0xEA, 0x5E}, //byteMatrix[1] => &0x0D
        {0xA1, 0x16, 0x00, 0xD0}  //byteMatrix[2] => &0xA1
    };

    //Vi henter ut verdiene:
    for(int row = 0; row < 3; row++)
    {
        for(int col = 0; col < 4; col++)
        {
            //Kan bruke standard syntaks, akkurat som i andre spraak:
            //printf("%02X", byteMatrix[row][col]);

            //...eller gjoere det vanskelig for aa understreke et poeng:
            //Leses som 'dereferert verdi av startadressa til byteMatrix pluss offset for kolonne og rad'.
            //I minnet er matrisa flat (altsaa endimensjonal), noe vi ser av adressene her.
            //De oeker nemlig med 1 for hvert element av matrisa vi skriver ut.
            printf("%02X | addr: %p\n", *(byteMatrix[0] + col + row*4), (byteMatrix[0] + col + row*4));

        }
        printf("\n");
    }
    
    printf("\nLister av strenger:\n");
    
    //Liste med 3 strenger av udefinert lengde. Som tidligere
    //peker stringList[n] til det foerste elementet i hver streng,
    //og er altsaa en char*
    char* stringList[3] =
    {
        "Alice",
        "Bob",
        "Geralt"
    };

    for(int i = 0; i < 3; i++)
    {
        //Lett:
        printf("%s\n", stringList[i]);

        //Vanskelig:
        char* stringChar = &stringList[i][0];
        while (*stringChar)
        {
            printf("%c", *stringChar);
            stringChar++;
        }
        printf("\n");

        //Egendefinert:
        print_string(&stringList[i][0]);
    }

    //Input-argumenter
    /**
     * Her kan vi illustrere bruk av input-argumenter!
     * Main-funksjonen tar inn to argumenter, int argc og char* argv[].
     * char* argv[] tolkes som en liste av strenger, og argc
     * sier hvor lang lista er. Det foerste argumentet er navnet
     * til programmet vaart, og vi henter derfor ut det andre
     * argumentet (indeks 1) for aa bruke det videre:
     */
    print_string(argv[1]); //Note to self: kall med 'hei verden'

    
    
    
    ////////////////////////////
    //Funksjonspekere og union//
    ////////////////////////////
    /**
     * Funksjonspekere er nyttige hvis man ikke
     * vet hvilken funksjon man skal kalle paa.
     * Merk at man maa angi typene til argumentene,
     * saa man er begrenset til tilfeller der
     * man har en kategori av funksjoner med
     * like argumenter og returverdi ("signatur").
     */
    void (*func_pointer)(char*);
    func_pointer = &print_string;
    func_pointer(&stringList[0][0]);


    //Union lar oss navngi subelementer av det samme minneomraadet
    Register_t registerEks;
    registerEks.integer = 0xDEADBEEF;
    for (int i = 0; i < 4; i++)
    {
        printf("Byte %d er %X\n", i, registerEks.bytes[i]);
    }
    //Vi ser nok en gang at endiannessen til minnesystemet gjoer
    //at innholdet av int'en tilsynelatende blir printet baklengs
    

    return 0;
}
