#include <stdio.h> //input-output
#include <string.h> //strenger
#include <stdint.h> //heltall med eksplisitt bredde
#include <stdlib.h> //diverse

#include "motordriver.h"



/**
 * I enkelte sammenhenger er det ikke mulig aa bruke
 * printf, f.eks. vil ikke alle mikrokontrollere ha
 * stoette for stdio. Da er det nyttig aa kunne lage
 * sin egen implementasjon av funksjonen. I de tilfellene
 * vil man typisk bytte ut kallet til printf i denne
 * funksjonen med noe som likner fra et bibliotek 
 * spesifikt for den mikrokontrolleren.
 */
void print_string(char* stringStart)
{
    //Kopier stringStart til en lokal variabel
    char* currentChar = stringStart;
    while(*currentChar)
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

    motor_t motor_test =
    {
        1,
        120,
        48,
        200
    };

    int16_t power = motor_get_power(&motor_test);

    printf("Power: %i\n", power);

    int32_t int1 = 345;
    int32_t* p_int1 = &int1;

    printf("address: %lx, val: %i\n", (int64_t)p_int1, *p_int1);
    printf("pointersize: %li\n", sizeof(p_int1));

    //Tekststrenger er egentlig lister av bokstaver:
    char tekststreng[] = "Hei paa deg din gamle sei\n";
    printf("Ren streng: %s", tekststreng);
    
    //Kan iterere gjennom lista, og bruke det faktum at strenger er NULL-terminerte for aa avbryte loopen
    printf("Som liste: ");
    //currentChar er en peker til en char som settes til addressa til tekststreng, som altsaa er en H
    char* currentChar = &tekststreng[0];
    //Loopen kjoerer saa lenge den derefererte verdien til currentChar ikke er NULL (NULL=0x00)
    //I forrige forelesning sa vi at while brukes naar vi ikke vet hvor mange iterasjoner
    //loopen skal kjoere, og dette er et godt eksempel paa det.
    while (*currentChar)
    {
        printf("%c", *currentChar); //Print ut den derefererte verdien av current, altsaa en character
        currentChar++;
    }
    //Naa gjoer vi det samme, men baklengs. Har ingen "nullterminering" i starten, saa vi maa 
    //soerge for at loopen kan stoppe i noe:
    printf("Baklengs som liste: ");
    char* first_char_of_string = &tekststreng[0];
    while (currentChar > first_char_of_string)
    {
        currentChar--; //Starter med a dekrementere, slik at pekeren peker mot den siste karakteren
        printf("%c", *currentChar);
    }
    printf("\n");
    
    //Eksempel paa at alt er data i C: Skrive ut bytene i en uint32 som en slags liste av uint8
    printf("uint32 per byte: ");
    uint32_t uint32Example = 0xDEADBEEF; //DEADBEEF er en mye brukt placeholder-verdi
    uint8_t* p_int32AsArray = (uint8_t*)&uint32Example;
    int num_uint8 = 4; //Det er 4 uint8 i en uint32
    int offset = 0;
    while(offset < num_uint8)
    {
        printf("%X", *(p_int32AsArray + offset));
        offset++;
    }
    printf("\n");
    //Dette avsloerer at minnet mitt er little endian: siste byte blir skrevet foerst.
    //Endianness er en av grunnene til at vi boer vaere forsiktige med aa
    //manipulere minne direkte. Normalt soerger kompilatoren for at
    //dette blir riktig, men her har vi tatt paa oss ansvaret for a hente ut dataene.
    //Vi maa kjoere loopen baklengs igjen for at dette skal gi mening:
    printf("uint32 per byte, baklengs: \n");
    while (offset > 0) //Husk at offset allerede er 4!
    {
        offset--;
        printf("%X", *(p_int32AsArray + offset));
    }
    printf("\n");

    
    //Her lagrer vi i stedet de samme bytene som en liste av uint8, 
    //og da blir de lagret i den rekkefoelgen vi forventer. Det er
    //rett og slett fordi at endianness kun gjelder for en enkelt variabel,
    //mens elementer i en liste blir lagret sekvensielt
    uint8_t byteArray[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    offset = 0;
    while (offset < 4)
    {
        printf("Addr: %p | Val: %X\n", &byteArray[offset], byteArray[offset]);
        offset++;
    }

    //Dobbeltpekere: Matriser, evt lister av strenger    
    uint8_t byteMatrix[3][4] =
    {
        {0xDE, 0xAD, 0xBE, 0xEF},
        {0x0D, 0x15, 0xEA, 0x5E},
        {0xA1, 0x16, 0x00, 0xD0}
    };

    int row = 0;
    for(int row = 0; row < 3; row++)
    {
        for(int col = 0; col < 4; col++)
        {
            //Innebygget matrisesyntaks:
            //printf("%02X", byteMatrix[row][col]);
            
            //Manuelt addressere hver enkelt byte:
            //Leses som "Dereferert verdi av adressa til byteMatrix[row] pluss offset for kolonne".
            //En matrise er en samling av pekere til lister, altsaa pekere til andre pekere til (tall-)verdier.
            //ByteMatrix[row] er i seg selv en addresse til det foerste elementet i raden,
            //derfor gir det mening aa legge kolonneindeksen til paa den.
            printf("%02X, addr: %p\n", *((byteMatrix[row] + col)), byteMatrix[row] + col);
            //Btw: %02X gir oss 2 heksadesimale sifre i store bokstaver; pen formatering
        }
        printf("\n");
    }

    //Liste med 3 strenger av udefinert lengde
    char* stringList[3] =
    {
        "Alice",
        "Bob",
        "Geralt",
    };

    for(int i = 0; i < 3; i++)
    {
        //Lett:
        printf("%s\n", stringList[i]);
        //Vanskelig (men ikke bedre):
        char* stringChar = &stringList[i][0];
        while(*stringChar)
        {
            printf("%c", *stringChar);
            stringChar++;
        }
        printf("\n");

        print_string(&stringList[i][0]);

    }

    //Input-argumenter
    /**
     * Naa skjoenner vi kanskje mer av hvordan input-argumenter
     * virker. char* argv[] tolkes som en liste av strenger,
     * og int argc sier hvor lang lista er. Det foerste 
     * "argumentet" er navnet paa programmet, vi henter derfor
     * ut det andre (indeks 1) for a bruke videre:
     */
    printf("Inputargument: %s\n", argv[1]);


    //Funksjonspekere er nyttige hvis man ikke vet hvilken
    //funksjon man skal kalle paa. Merk at man maa angi
    //typene til argumentene, saa man er begrenset til
    //tilfeller der man har en kjent kategori funksjoner.
    void (*func_pointer)(char*);
    func_pointer = &print_string;
    func_pointer(&stringList[0][0]);

    //Union: navngi subelementer av det samme minneomraadet.
    Register_t registerEks;
    registerEks.integer = 0xDEADBEEF;
    for(int i = 0; i < 4; i++)
    {
        printf("Byte %d er %x\n", i, registerEks.bytes[i]);
    }
    //Ser nok en gang at endiannessen til minnesystemet gjoer
    //at innholdet av int'en tilsynelatende blir printet baklengs




    return 0;
}
