#include <stdio.h>
#include <stdlib.h>
#include <hwloc.h>
#include <papi.h>

int main() {
    int level, numlevels;

    /* Variaveis das medicoes reais (hwloc) */
    hwloc_uint64_t real_cache[3];
    unsigned real_line[3];
    hwloc_topology_t topology;
    hwloc_obj_t obj;

    /* Variaveis das medicoes estimadas (PAPI) */
    int events[3] = { PAPI_L1_DCM, PAPI_L2_DCM, PAPI_L3_DCM };
    long long misses[3];
    int papilevels = 3;

    /* Inicializa hwloc */
    if (hwloc_topology_init(&topology)) {
        fprintf(stderr, "Erro em hwloc_topology_init\n");
        exit(1);
    }
    hwloc_topology_load(topology);

    /* Pega tamanhos reais de cache e line */
    level = 0;
    for (obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 0); obj; obj = obj->parent) {
        if (obj->type == HWLOC_OBJ_CACHE) {
            real_cache[level] = obj->attr->cache.size;
            real_line[level] = obj->attr->cache.linesize;
            level++;
        }
    }
    numlevels = level;

    /* Comeca a contar eventos */
    while (PAPI_start_counters(events, papilevels) != PAPI_OK) {
        papilevels--;
    }

    /* TODO: alguma coisa para estimar */

    /* Le os contadores */
    if (PAPI_read_counters(misses, papilevels) != PAPI_OK) {
        fprintf(stderr, "Erro em PAPI_read_counters\n");
        exit(1);
    }

    for (level = 0; level < papilevels; level++) {
        printf("L%d cache miss: %lld\n", level+1, misses[level]);
    }
    printf("\n");

    /* Imprime tabela */
    printf("Nível Linha Real Cache Real Linha Est. Cache Est.\n");
    printf("----- ---------- ---------- ---------- ----------\n");
    for (level = 0; level < 3; level++) {
        printf("%5d ", level+1);
        if (level < numlevels) {
            printf("%10u ", real_line[level]);
            printf("%9lluK ", (unsigned long long) real_cache[level] / 1024);
            if (level < papilevels) {
                /* TODO: imprimir valores estimados no lugar dos reais */
                printf("%10u ", real_line[level]);
                printf("%9lluK", (unsigned long long) real_cache[level] / 1024);
            } else {
                printf("       N/D        N/D");
            }
        } else {
            printf("       N/D        N/D        N/D        N/D");
        }
        printf("\n");
    }
    return 0;
}

