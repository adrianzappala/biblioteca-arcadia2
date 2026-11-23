#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "reportes.h"
#include "archivosPrestamos.h"
#include "archivosSocios.h"
#include "archivosLibros.h"
#include "autores.h"

#define TOP 10
#define MAX_GENEROS 100

typedef struct {
    char clave[100];
    int cantidad;
} Conteo;

/* Arreglo dinamico de conteos */
typedef struct {
    Conteo *v;
    int n;
} Conteos;

/* Todos los datos que usan los reportes, cargados juntos */
typedef struct {
    Prestamo *prestamos;
    int nPrestamos;
    Libro *libros;
    int nLibros;
    Socio *socios;
    int nSocios;
} Datos;


static void liberarDatos(Datos *d)
{
    liberarPrestamos(&d->prestamos, &d->nPrestamos);
    liberarLibros(&d->libros, &d->nLibros);
    liberarSocios(&d->socios, &d->nSocios);
}

/* Carga prestamos, libros y socios. Devuelve 0 si falta memoria */
static int cargarDatos(Datos *d)
{
    d->prestamos = NULL;
    d->libros = NULL;
    d->socios = NULL;
    if (cargarPrestamos(&d->prestamos, &d->nPrestamos) &&
        cargarLibros(&d->libros, &d->nLibros) &&
        cargarSocios(&d->socios, &d->nSocios))
    {
        return 1;
    }
    printf("Error de memoria al cargar los datos.\n");
    liberarDatos(d);
    return 0;
}

/* ---------- Conteos ---------- */

/* Suma 'valor' a la clave. Si la clave no estaba, la agrega al final */
static void sumar(Conteos *c, const char clave[], int valor)
{
    int i;
    Conteo *nuevo;

    for (i = 0; i < c->n; i++)
    {
        if (strcmp(c->v[i].clave, clave) == 0)
        {
            c->v[i].cantidad = c->v[i].cantidad + valor;
            return;
        }
    }

    /* realloc puede mover el arreglo, por eso se guarda en otra variable */
    nuevo = (Conteo *)realloc(c->v, (c->n + 1) * sizeof(Conteo));
    if (nuevo == NULL)
    {
        printf("Error de memoria.\n");
        return;
    }
    c->v = nuevo;
    strcpy(c->v[c->n].clave, clave);
    c->v[c->n].cantidad = valor;
    c->n++;
}

/* Ordenamiento por seleccion. descendente = 1 ordena de mayor a menor, 0 de menor a mayor */
static void ordenar(Conteos *c, int descendente)
{
    int i, j, mejor;
    Conteo aux;

    for (i = 0; i < c->n - 1; i++)
    {
        mejor = i;
        for (j = i + 1; j < c->n; j++)
        {
            if (descendente == 1 && c->v[j].cantidad > c->v[mejor].cantidad)
            {
                mejor = j;
            }
            if (descendente == 0 && c->v[j].cantidad < c->v[mejor].cantidad)
            {
                mejor = j;
            }
        }
        aux = c->v[i];
        c->v[i] = c->v[mejor];
        c->v[mejor] = aux;
    }
}


static void limpiarEntrada(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}

/* Pide un anio. Devuelve 0 si no es valido */
static int pedirAnio(void)
{
    int anio;

    printf("Anio: ");
    if (scanf("%d", &anio) != 1 || anio < 1900 || anio > 3000)
    {
        limpiarEntrada();
        printf("Anio invalido.\n");
        return 0;
    }
    limpiarEntrada();
    return anio;
}

static int anioDe(time_t fecha)
{
    struct tm *t = localtime(&fecha);
    return t->tm_year + 1900;
}


static const char *generoDeLibro(const Libro *libro)
{
    int i;

    for (i = 0; i < cantidadObras; i++)
    {
        if (libro->idAutor == obras[i].idAutor && strcmp(libro->titulo, obras[i].titulo) == 0)
        {
            return obras[i].tipo;
        }
    }
    return "Sin genero";
}


static const char *nombreSocio(const Datos *d, const char idSocio[])
{
    int pos = buscarSocioEnLista(d->socios, d->nSocios, idSocio);

    if (pos == -1)
    {
        return "(no encontrado)";
    }
    return d->socios[pos].nombre;
}


static void contarPorIsbn(const Datos *d, int anio, int todos, Conteos *c)
{
    int i;

    if (todos == 1)
    {
        for (i = 0; i < d->nLibros; i++)
        {
            if (d->libros[i].descatalogado == 0)
            {
                sumar(c, d->libros[i].isbn, 0);
            }
        }
    }
    for (i = 0; i < d->nPrestamos; i++)
    {
        if (anio == 0 || anioDe(d->prestamos[i].fechaPrestamo) == anio)
        {
            sumar(c, d->prestamos[i].isbn, 1);
        }
    }
}

/* Muestra titulo y autor de un ISBN */
static void imprimirLibro(const Datos *d, const char isbn[])
{
    int pos = buscarLibroEnLista(d->libros, d->nLibros, isbn);

    if (pos == -1)
    {
        printf("(no esta en el catalogo)");
    }
    else
    {
        printf("%-35s %-22s", d->libros[pos].titulo, obtenerNombreAutor(d->libros[pos].idAutor));
    }
}

static void mostrarRankingLibros(const Datos *d, const Conteos *c)
{
    int i;

    printf("\n%-3s %-9s %-35s %-22s %s\n", "#", "ISBN", "TITULO", "AUTOR", "PRESTAMOS");
    for (i = 0; i < c->n && i < TOP; i++)
    {
        printf("%-3d %-9s ", i + 1, c->v[i].clave);
        imprimirLibro(d, c->v[i].clave);
        printf(" %d\n", c->v[i].cantidad);
    }
}


static void mostrarRankingTexto(const Conteos *c, const char titulo[])
{
    int i;

    printf("\n%s\n", titulo);
    for (i = 0; i < c->n && i < TOP; i++)
    {
        printf("%-3d %-45s %d\n", i + 1, c->v[i].clave, c->v[i].cantidad);
    }
}

/* ---------- Reportes de libros ---------- */

void reporteLibrosMasPrestados(void)
{
    Datos d;
    Conteos c = {NULL, 0};

    printf("\n--- LIBROS MAS PRESTADOS ---\n");
    if (cargarDatos(&d) == 0)
    {
        return;
    }
    contarPorIsbn(&d, 0, 0, &c);
    if (c.n == 0)
    {
        printf("Todavia no hay prestamos registrados.\n");
    }
    else
    {
        ordenar(&c, 1);
        mostrarRankingLibros(&d, &c);
    }
    free(c.v);
    liberarDatos(&d);
}

void reporteLibrosMenosPrestados(void)
{
    Datos d;
    Conteos c = {NULL, 0};
    int i, sinPrestamos = 0;

    printf("\n--- LIBROS MENOS SOLICITADOS ---\n");
    if (cargarDatos(&d) == 0)
    {
        return;
    }
    contarPorIsbn(&d, 0, 1, &c);
    ordenar(&c, 0);
    mostrarRankingLibros(&d, &c);
    for (i = 0; i < c.n; i++)
    {
        if (c.v[i].cantidad == 0)
        {
            sinPrestamos++;
        }
    }
    printf("\nLibros que nunca se prestaron: %d\n", sinPrestamos);
    free(c.v);
    liberarDatos(&d);
}

void reporteBestsellerAnual(void)
{
    Datos d;
    Conteos c = {NULL, 0};
    int anio, i;

    printf("\n--- BESTSELLER ANUAL ---\n");
    anio = pedirAnio();
    if (anio == 0 || cargarDatos(&d) == 0)
    {
        return;
    }
    contarPorIsbn(&d, anio, 0, &c);
    if (c.n == 0)
    {
        printf("No hubo prestamos en %d.\n", anio);
    }
    else
    {
        ordenar(&c, 1);
        printf("\nBestseller %d (%d prestamos):\n", anio, c.v[0].cantidad);
        /* Si hay empate en el primer puesto se muestran todos */
        for (i = 0; i < c.n && c.v[i].cantidad == c.v[0].cantidad; i++)
        {
            imprimirLibro(&d, c.v[i].clave);
            printf("\n");
        }
    }
    free(c.v);
    liberarDatos(&d);
}

void reporteRankingAutoresGeneros(void)
{
    Datos d;
    Conteos autores = {NULL, 0};
    Conteos generos = {NULL, 0};
    int i, pos;

    printf("\n--- RANKING DE AUTORES Y GENEROS ---\n");
    if (cargarDatos(&d) == 0)
    {
        return;
    }
    for (i = 0; i < d.nPrestamos; i++)
    {
        pos = buscarLibroEnLista(d.libros, d.nLibros, d.prestamos[i].isbn);
        if (pos != -1)
        {
            sumar(&autores, obtenerNombreAutor(d.libros[pos].idAutor), 1);
            sumar(&generos, generoDeLibro(&d.libros[pos]), 1);
        }
    }
    if (autores.n == 0)
    {
        printf("Todavia no hay prestamos registrados.\n");
    }
    else
    {
        ordenar(&autores, 1);
        ordenar(&generos, 1);
        mostrarRankingTexto(&autores, "AUTORES MAS PRESTADOS");
        mostrarRankingTexto(&generos, "GENEROS MAS PRESTADOS");
    }
    free(autores.v);
    free(generos.v);
    liberarDatos(&d);
}

void reporteMatrizGeneroMes(void)
{
    Datos d;
    Conteos generos = {NULL, 0};         /* la posicion de cada genero es su fila en la matriz */
    int matriz[MAX_GENEROS][12];
    char *meses[12] = {"Ene", "Feb", "Mar", "Abr", "May", "Jun",
                       "Jul", "Ago", "Sep", "Oct", "Nov", "Dic"};
    int anio, i, j, pos, fila, total = 0;
    const char *genero;
    time_t fecha;

    printf("\n--- PRESTAMOS POR GENERO Y MES ---\n");
    anio = pedirAnio();
    if (anio == 0 || cargarDatos(&d) == 0)
    {
        return;
    }
    memset(matriz, 0, sizeof(matriz));
    for (i = 0; i < d.nPrestamos; i++)
    {
        fecha = d.prestamos[i].fechaPrestamo;
        pos = buscarLibroEnLista(d.libros, d.nLibros, d.prestamos[i].isbn);
        if (pos == -1 || anioDe(fecha) != anio)
        {
            continue;
        }
        genero = generoDeLibro(&d.libros[pos]);
        sumar(&generos, genero, 1);
        /* la fila del genero es su posicion en la lista */
        for (fila = 0; fila < generos.n && strcmp(generos.v[fila].clave, genero) != 0; fila++);
        if (fila < generos.n && fila < MAX_GENEROS)
        {
            matriz[fila][localtime(&fecha)->tm_mon]++;
            total++;
        }
    }

    if (total == 0)
    {
        printf("No hubo prestamos en %d.\n", anio);
    }
    else
    {
        printf("\n%-30s", "GENERO");
        for (j = 0; j < 12; j++)
        {
            printf(" %3s", meses[j]);
        }
        printf("\n");
        for (i = 0; i < generos.n && i < MAX_GENEROS; i++)
        {
            printf("%-30s", generos.v[i].clave);
            for (j = 0; j < 12; j++)
            {
                printf(" %3d", matriz[i][j]);
            }
            printf("\n");
        }
    }
    free(generos.v);
    liberarDatos(&d);
}

/* ---------- Reportes de socios y prestamos ---------- */

void reporteSociosMasActivos(void)
{
    Datos d;
    Conteos c = {NULL, 0};
    int i;

    printf("\n--- SOCIOS MAS ACTIVOS ---\n");
    if (cargarDatos(&d) == 0)
    {
        return;
    }
    for (i = 0; i < d.nPrestamos; i++)
    {
        sumar(&c, d.prestamos[i].idSocio, 1);
    }
    if (c.n == 0)
    {
        printf("Todavia no hay prestamos registrados.\n");
    }
    else
    {
        ordenar(&c, 1);
        printf("\n%-3s %-8s %-30s %s\n", "#", "ID", "NOMBRE", "PRESTAMOS");
        for (i = 0; i < c.n && i < TOP; i++)
        {
            printf("%-3d %-8s %-30s %d\n", i + 1, c.v[i].clave,
                   nombreSocio(&d, c.v[i].clave), c.v[i].cantidad);
        }
    }
    free(c.v);
    liberarDatos(&d);
}

void reporteMoras(void)
{
    Datos d;
    Prestamo p;
    time_t hoy = time(NULL);
    int i, vencidos = 0;
    char vence[11];

    printf("\n--- PRESTAMOS VENCIDOS ---\n");
    if (cargarDatos(&d) == 0)
    {
        return;
    }
    printf("\n%-5s %-8s %-22s %-10s %-11s %s\n", "ID", "SOCIO", "NOMBRE", "ISBN", "VENCIO", "DIAS DE MORA");
    for (i = 0; i < d.nPrestamos; i++)
    {
        p = d.prestamos[i];
        /* Mora: no fue devuelto y la fecha de vencimiento ya paso */
        if (p.devuelto == 0 && hoy > p.fechaVencimiento)
        {
            strftime(vence, sizeof(vence), "%d/%m/%Y", localtime(&p.fechaVencimiento));
            printf("%-5d %-8s %-22s %-10s %-11s %ld\n", p.idPrestamo, p.idSocio,
                   nombreSocio(&d, p.idSocio), p.isbn, vence,
                   (long)((hoy - p.fechaVencimiento) / 86400));
            vencidos++;
        }
    }
    if (vencidos == 0)
    {
        printf("No hay prestamos vencidos sin devolver.\n");
    }
    liberarDatos(&d);
}

void reporteEstadoInventario(void)
{
    Datos d;
    int i, total = 0, disponibles = 0, descatalogados = 0, activos = 0;

    printf("\n--- ESTADO DEL INVENTARIO ---\n");
    if (cargarDatos(&d) == 0)
    {
        return;
    }
    for (i = 0; i < d.nLibros; i++)
    {
        total = total + d.libros[i].stockTotal;
        disponibles = disponibles + d.libros[i].stockDisponible;
        if (d.libros[i].descatalogado == 1)
        {
            descatalogados++;
        }
    }
    for (i = 0; i < d.nPrestamos; i++)
    {
        if (d.prestamos[i].devuelto == 0)
        {
            activos++;
        }
    }

    printf("Titulos en el catalogo:        %d\n", d.nLibros - descatalogados);
    printf("Titulos descatalogados:        %d\n", descatalogados);
    printf("Ejemplares totales:            %d\n", total);
    printf("Ejemplares disponibles:        %d\n", disponibles);
    printf("Ejemplares prestados:          %d\n", total - disponibles);
    printf("Prestamos activos registrados: %d\n", activos);
    if (total - disponibles != activos)
    {
        printf("\nAVISO: los ejemplares prestados no coinciden con los prestamos activos.\n");
    }
    liberarDatos(&d);
}

/* ---------- Menu ---------- */

void menuReportes(void)
{
    int opcion;

    do
    {
        printf("\n=== REPORTES ARCADIA ===\n");
        printf("1. Libros mas prestados\n");
        printf("2. Bestseller del anio\n");
        printf("3. Ranking de autores y generos\n");
        printf("4. Socios mas activos\n");
        printf("5. Prestamos vencidos\n");
        printf("6. Estado del inventario\n");
        printf("7. Libros menos solicitados\n");
        printf("8. Prestamos por genero y mes\n");
        printf("0. Volver\n");
        printf("Seleccione una opcion: ");
        if (scanf("%d", &opcion) != 1)
        {
            opcion = -1;
        }
        limpiarEntrada();

        switch (opcion)
        {
            case 1: reporteLibrosMasPrestados(); break;
            case 2: reporteBestsellerAnual(); break;
            case 3: reporteRankingAutoresGeneros(); break;
            case 4: reporteSociosMasActivos(); break;
            case 5: reporteMoras(); break;
            case 6: reporteEstadoInventario(); break;
            case 7: reporteLibrosMenosPrestados(); break;
            case 8: reporteMatrizGeneroMes(); break;
            case 0: break;
            default: printf("Opcion invalida.\n");
        }
    }
    while (opcion != 0);
}
