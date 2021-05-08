#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
int THR_COUNT = 0;

void parse(char *line, char **argv)
{
    while (*line != '\0') /*   cümlenin sonuna gelene kadar döner.      */
    {

        while (*line == ' ' || *line == '\t' || *line == '\n')
        {
            *line++ = '\0'; /* boşlukları \0 ile değiştirir.             */
        }

        *argv++ = line; /* argüman pozisyonu kaydedilir       */

        while (*line != '\0' && *line != ' ' &&
               *line != '\t' && *line != '\n')
        {
            if (*line == ';')
            {
                *line++ = '\0'; /*';' ü null yap ve ilerle       */
                *argv = '\0';

                if (*line != '\0')
                    THR_COUNT++; /*';' görüldüğünde thread sayısı 1 artacak.     */

                *argv++;
                *argv++ = line; /*bir sonraki parametre argv'ye atanır ve argv ilerletilir.  */
            }
            else
            {
                line++; /* while koşulundaki değerleri görene kadar ilerle*/
            }
        }
    }
    if (*line != ';')
    {
        *argv = '\0'; /* argümanların sonuna gelindiğini işaretler  */
        THR_COUNT++;
    }
}

void *execute(char **argv)
{

    int status;
    pid_t pid;

    if ((pid = fork()) < 0)
    { /*çocuk process için fork oluşturulur.          */
        printf("*** Çocuk process başarısız oldu.\n");
        exit(1);
    }
    else if (pid == 0)
    { /* çocuk process         */
        if (execvp(*argv, argv) < 0)
        { /* komutu çalıştırır  */
            printf("***Komut Bulunamadı\n");
            exit(1);
        }
    }
    else
    {
        while (wait(&status) != pid) /* ebeveyn processin işinin bitirilmesi beklenir.  */
            ;
    }
}

void Threadle(char **argv)
{
    int argvCounter = 0;

    pthread_t tid[THR_COUNT];
    pthread_attr_t attr;
    int i;

    pthread_attr_init(&attr);
    for (i = 0; i < THR_COUNT; i++)
    {
        int mallocCounter = 0;                                     /*     gecici dizimizin indexlerini tutar    */
        char **malocGecici = (char **)malloc(10 * sizeof(char *)); /*thread parametlerine atmak için geçici dizi*/

        /*   gelen toplu argümanları kısım kısım ayırıp threadlere parametre olarak gönderilir.      */
        while (argv[argvCounter] != NULL)
        {
            malocGecici[mallocCounter] = argv[argvCounter];
            // printf("%s \n", malocGecici[mallocCounter]);

            mallocCounter++;
            argvCounter++;
        }
        malocGecici[mallocCounter] = NULL;

        pthread_create(&tid[i], &attr, execute, (char *)malocGecici); /*  tüm komutları eş zamanlı çalıştırmak üzere thread processine yollar   */
        sleep(1);
        argvCounter++;
    }
    for (i = 0; i < THR_COUNT; i++)
    {

        pthread_join(tid[i], NULL); /*      buradaki null return değeri     */
    }
}
void fonk()
{
}
void main(void)
{
    uid_t uid;
    char *username;
    uid = getuid();
    username = getlogin();
    char hostname[HOST_NAME_MAX + 1];
    gethostname(hostname, HOST_NAME_MAX + 1);
    char line[1024]; /* prompt satırı                */
    char *argv[64];  /* prompt argümanları/parametreleri     */
    char *argv2[64]; /* "shell" ile dosya okutulduğunda kullanılacak olan argüman dizisi*/

    int pid, fd2, boyut;
    struct stat st;
    char g[100], *veri; //dosya ismi

    boyut = (int)st.st_size;
    int status;
    while (1)
    {
        signal(SIGINT, fonk);                       /* promptun sürekliliği sağlanır         */
        printf("(%s@%s)\n$  ", username, hostname); /*   prompt görüntülenir.             */
        gets(line);                                 /*   kullanıcıdan komut/komutlar alınır.   */

        if (line[0] != NULL)
        {
            parse(line, argv); /*   satırı parçalar               */

            if (strcmp(argv[0], "shell") == 0)
            {

                fd2 = open(argv[1], O_RDWR); //dosya yoksa hata verdir.

                pid = fork();
                if (pid == 0)
                {
                    exit(1);
                }
                else
                {
                    fstat(fd2, &st);                             /*dosya özellikleri struct dan dosyaya ait tüm özellikleri çeker*/
                    boyut = (int)st.st_size;                     /*st structdan dosya boyutunu çeker*/
                    veri = (char *)malloc(sizeof(char) * boyut); /*dosya boyutu kadar heap bellekte yer açılır.*/
                    lseek(fd2, 0, SEEK_SET);                     /*pointer dosyanın başına alınır*/

                    read(fd2, veri, boyut);   /*dosya okunur*/
                    parse(veri, argv2);       /* okunan dosya komutlara ayrılmak üzere parse a yollanır.*/
                    Threadle(argv2);          /*threadle komutları eş zamanlı çalıştırır.*/
                    close(fd2);               /*dosya kapatılır*/
                    free(veri);               /*dosyanın bellekte kapladığı yer serbest bırakılır.*/
                    waitpid(pid, 0, &status); /*parent process öldürülür.*/
                }
            }
            else
            {
                if (strcmp(argv[0], "exit") == 0)
                    exit(0);

                Threadle(argv); /*   parçalanan komut satırı argümanları çalışıtırılmak için(gerekirse eş zamanlı) gönderilir.   */
                THR_COUNT = 0;  /*    işlemler sonunda thread kalmadığı için thread sıfırlanır    */
            }
        }
    }
}