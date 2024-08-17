//
// unzip test
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unzip/unzipLIB.h>

void *myOpen(const char *filename, int32_t *size)
{
    printf("Attempting to open %s\n", filename);
    FILE *f;
    size_t filesize;
    f = fopen(filename, "r+b");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        filesize = ftell(f);
        fseek(f, 0, SEEK_SET);
        *size = (int32_t)filesize;
    }
    return (void *)f;
} /* myOpen() */

void myClose(void *p)
{
    ZIPFILE *pzf = (ZIPFILE *)p;
    if (pzf)
        fclose((FILE *)pzf->fHandle);
} /* myClose() */

int32_t myRead(void *p, uint8_t *buffer, int32_t length)
{
    ZIPFILE *pzf = (ZIPFILE *)p;
    if (!pzf)
        return 0;
    return (int32_t)fread(buffer, 1, length, (FILE *)pzf->fHandle);
} /* myRead() */

int32_t mySeek(void *p, int32_t position, int type)
{
    ZIPFILE *pzf = (ZIPFILE *)p;
    if (!pzf)
        return 0;
    return fseek((FILE *)pzf->fHandle, position, type);
}

int main(int argc, const char *argv[])
{
    int rc, i;
    //   unzFile zHandle;
    char szTemp[256];
    ZIPFILE zpf;
    UNZIP zHandle;

    if (argc != 3)
    {
        printf("Usage: unziptest <zip file> <file to unzip within the zip>\n");
        printf("       or unziptest (no arguments) to test mem to mem unzipping\n");
        return 0;
    }

    printf("Starting unzip test...reading file %s from zip archive %s\n", argv[2], argv[1]);

    int err = zHandle.openZIP(argv[1], myOpen, myClose, myRead, mySeek);

    if (err)
    {
        printf("Error opening file: %s\n", argv[1]);
        return -1;
    }
    // Display the global comment (if any)
    rc = zHandle.getGlobalComment(szTemp, sizeof(szTemp));
    printf("Global Comment: %s\n", szTemp);

    rc = zHandle.locateFile(argv[2]);

    if (rc != UNZ_OK) /* Report the file not found */
    {
        printf("file %s not found within archive\n", argv[2]);
        zHandle.closeZIP();
        return -1;
    }

    rc = zHandle.openCurrentFile(); /* Try to open the file we want */
    if (rc != UNZ_OK)
    {
        printf("Error opening file = %d\n", rc);
        zHandle.closeZIP();
        return -1;
    }

    printf("File located within archive.\n");
    rc = 1;
    i = 0;

    while (rc > 0)
    {
        rc = zHandle.readCurrentFile((uint8_t *)szTemp, sizeof(szTemp) - 1);

        if (rc == 0)
        {
            break;
        }
        else if (rc > 0)
        {
            szTemp[rc] = '\0';
            printf(szTemp);
            i += rc;
        }
        else
        {
            printf("Error reading from file\n");
            break;
        }
    }
    
    puts("");

    printf("Total bytes read = %d (reading 256 bytes at a time)\n", i);
    rc = zHandle.closeCurrentFile();
    zHandle.closeZIP();

    return 0;
} /* main() */
