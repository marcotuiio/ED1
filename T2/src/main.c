#include "libs.h"
#include "paths.h"
#include "system.h"
#include "tree.h"
#include "geo.h"
#include "svg.h"

int main(int argc, char** argv) {
    Paths my_paths = createAllPaths();
    Tree my_tree = createTree();

    readParam(argc, argv, my_paths);

    // printf("\n");
    // puts(getBed(my_paths));
    // puts(getBsd(my_paths));

    // puts(getBedGeo(my_paths));
    // puts(getGeoArq(my_paths));
    // puts(getGeoName(my_paths));

    // puts(getBedQry(my_paths));
    // puts(getQryArq(my_paths));
    // puts(getQryName(my_paths));

    // puts(getBsdGeoSvg(my_paths));
    // puts(getBsdGeoQrySvg(my_paths));
    // puts(getBsdGeoQryTxt(my_paths));
    // printf("\n");

    readGeo(getBedGeo(my_paths), my_tree); 

    writeSvg(getBsdGeoSvg(my_paths), my_tree);

    freeTree(getRoot(my_tree));
    free(my_tree);
    freePaths(my_paths);

    return 0;
}