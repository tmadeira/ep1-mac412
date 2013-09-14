#include <stdio.h>
#include <stdlib.h>
#include <hwloc.h>

int main() {
    int level, maxlevel;
    hwloc_uint64_t cache[3];
    unsigned line[3];
    hwloc_topology_t topology;
    hwloc_obj_t obj;
    
    /* Inicializa hwloc */
    if (hwloc_topology_init(&topology)) {
        exit(1);
    }
    hwloc_topology_load(topology);

    /* Pega tamanhos reais de cache e line */
    level = 0;
    for (obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 0); obj; obj = obj->parent) {
        if (obj->type == HWLOC_OBJ_CACHE) {
            cache[level] = obj->attr->cache.size;
            line[level] = obj->attr->cache.linesize;
            level++;
        }
    }
    maxlevel = maxlevel;

    /* Imprime tabela */
    printf("Nível Linha Real Cache Real Linha Est. Cache Est.\n");
    printf("----- ---------- ---------- ---------- ----------\n");
    for (level = 0; level < 3; level++) {
        printf("%5d ", level+1);
        printf("%10u ", line[level]);
        printf("%9lluK ", (unsigned long long) cache[level] / 1024);
        printf("\n");
    }
    return 0;
}

