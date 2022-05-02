#include "libs.h"

#include "geo.h"
#include "qry.h"
#include "system.h"

int main(int argc, char **argv) {
    FILE *qryFile;

    Parameters param = createParameters(argc, argv);

    char *pathGeo = makePathGeoFile(param);

    FILE *geoFile = loadFile(pathGeo);

    if (geoFile == NULL) {
        printf("Geo File NULO\n");
        return 0;
    }

    char *outputDir = getOutputDir(param);

    if (qryExiste(param) == 1) {
        char *pathQry = makePathQryFile(param);
        int size = strlen(pathQry);
        char *path_aux = malloc(size);
        strcpy(path_aux, pathQry);
        
        qryFile = loadFile(pathQry);

        int q_size = countInp(path_aux);
        printf("SIZE QUEUE %d\n", q_size);

        buildGeometricForms(geoFile, outputDir, qryFile, 1, q_size);
        free(path_aux);
        free(pathQry);

    } else {
        buildGeometricForms(geoFile, outputDir, NULL, 0, 0);
    }

    free(param);
    free(pathGeo);
    free(outputDir);

    printf("\n--- END OF MAIN SUCCESS ---\n");

    return 0;
}
