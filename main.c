#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "autores.h"
#include "libros.h"
#include "socios.h"
#include "prestamos.h"
#include "estructuras.h"
#include "reportes.h"

/* Busca el ID de un socio a partir de su nombre */
int obtenerIdSocio(const char nombre[], char idSocio[])
{
    FILE *archivo;
    Socio socio;

    archivo = fopen("socios.dat", "rb");

    if (archivo == NULL)
    {
        return 0;
    }

    while (fread(&socio, sizeof(Socio), 1, archivo) == 1)
    {
        if (strcmp(socio.nombre, nombre) == 0)
        {
            strcpy(idSocio, socio.idSocio);
            fclose(archivo);
            return 1;
        }
    }

    fclose(archivo);
    return 0;
}

/* Submenu de gestion de socios */
void menuSocios(void)
{
    int opcion;

    do
    {
        printf("\n===== GESTION DE SOCIOS =====\n");
        printf("1. Registrar socio\n");
        printf("2. Listar socios\n");
        printf("3. Buscar socio por ID\n");
        printf("4. Buscar socio por DNI\n");
        printf("5. Modificar socio\n");
        printf("6. Cambiar estado de socio\n");
        printf("0. Volver al menu principal\n");

        printf("\nSeleccione una opcion: ");

        if (scanf("%d", &opcion) != 1)
        {
            limpiarBuffer();
            opcion = -1;
        }

        switch (opcion)
        {
            case 1:
                registrarSocio();
                break;

            case 2:
                listarSocios();
                break;

            case 3:
                buscarSocioPorId();
                break;

            case 4:
                buscarSocioPorDni();
                break;

            case 5:
                modificarSocio();
                break;

            case 6:
                cambiarEstadoSocio();
                break;

            case 0:
                printf("Volviendo al menu principal...\n");
                break;

            default:
                printf("Opcion incorrecta.\n");
        }

    } while (opcion != 0);
}

/* Menu principal para empleados */
void menuEmpleado(void)
{
    int opcion;
    int idAutor;

    do
    {
        printf("\n===== BIBLIOTECA ARCADIA =====\n");
        printf("1. Agregar libro\n");
        printf("2. Modificar libro\n");
        printf("3. Listar libros\n");
        printf("4. Buscar libro por ISBN\n");
        printf("5. Listar libros disponibles\n");
        printf("6. Listar autores\n");
        printf("7. Consultar obras de un autor\n");
        printf("8. Cargar obras iniciales al catalogo\n");
        printf("9. Buscar libro por titulo\n");
        printf("10. Buscar libros por autor\n");
        printf("11. Buscar libros por genero\n");
        printf("12. Buscar libros por anio\n");
        printf("13. Buscar libros por inicial\n");
        printf("14. Listar libros ordenados alfabeticamente\n");
        printf("15. Mostrar historial de busquedas\n");
        printf("16. Resumen de stock por autor\n");
        printf("17. Listar libros con memoria dinamica\n");
        printf("18. Listar libros recursivamente\n");
        printf("19. Sistema de recomendaciones\n");
        printf("-----------------------------------\n");
        printf("20. Gestion de socios\n");
        printf("21. Gestion de prestamos\n");
        printf("22. Reportes\n");
        printf("0. Salir\n");

        printf("\nSeleccione una opcion: ");

        if (scanf("%d", &opcion) != 1)
        {
            limpiarBuffer();
            opcion = -1;
        }

        switch (opcion)
        {
            case 1:
                agregarLibro();
                break;

            case 2:
                modificarLibro();
                break;

            case 3:
                listarLibros();
                break;

            case 4:
                buscarLibro();
                break;

            case 5:
                listarLibrosDisponibles();
                break;

            case 6:
                listarAutores();
                break;

            case 7:
                printf("Ingrese el ID del autor: ");
                scanf("%d", &idAutor);
                listarObrasAutor(idAutor);
                break;

            case 8:
                cargarObrasIniciales();
                break;

            case 9:
                buscarLibroPorTitulo();
                break;

            case 10:
                buscarLibrosPorAutor();
                break;

            case 11:
                buscarLibrosPorGenero();
                break;

            case 12:
                buscarLibrosPorAnio();
                break;

            case 13:
                buscarLibrosPorInicial();
                break;

            case 14:
                listarLibrosOrdenados();
                break;

            case 15:
                mostrarHistorialBusquedas();
                break;

            case 16:
                mostrarResumenStockAutores();
                break;

            case 17:
                listarLibrosMemoriaDinamica();
                break;

            case 18:
                listarLibrosRecursivo();
                break;

            case 19:
                recomendarLibros();
                break;

            case 20:
                menuSocios();
                break;

            case 21:
                menuPrestamos();
                break;

            case 22:
                menuReportes();
                break;

            case 0:
                printf("Saliendo del sistema...\n");
                break;

            default:
                printf("Opcion incorrecta.\n");
        }

    } while (opcion != 0);
}

/* Menu para socios */
void menuSocio(const char nombre[], const char idSocio[])
{
    int opcion;

    do
    {
        printf("\n===== BIBLIOTECA ARCADIA - SOCIO =====\n");
        printf("Bienvenido/a, %s\n", nombre);
        printf("1. Consultar mis datos personales\n");
        printf("2. Consultar mis prestamos\n");
        printf("0. Cerrar sesion\n");

        printf("\nSeleccione una opcion: ");

        if (scanf("%d", &opcion) != 1)
        {
            limpiarBuffer();
            opcion = -1;
        }

        switch (opcion)
        {
            case 1:
                verMisDatos(nombre);
                break;

            case 2:
                listarPrestamos(LISTAR_TODOS, idSocio);
                break;

            case 0:
                printf("Cerrando sesion...\n");
                break;

            default:
                printf("Opcion incorrecta.\n");
        }

    } while (opcion != 0);
}

int main(void)
{
    int tipoUsuario;
    int reintentar;
    char nombreLogueado[50];
    char idSocio[10];

    /* Crea el usuario administrador si todavia no existe */
    inicializarUsuarioAdmin();

    printf("\n====================================\n");
    printf("       BIBLIOTECA ARCADIA\n");
    printf("====================================\n");

    do
    {
        tipoUsuario = iniciarSesion(nombreLogueado);

        if (tipoUsuario == 0)
        {
            printf("\n1. Intentar nuevamente\n");
            printf("0. Salir\n");
            printf("Seleccione una opcion: ");

            if (scanf("%d", &reintentar) != 1)
            {
                limpiarBuffer();
                reintentar = 0;
            }

            if (reintentar == 0)
            {
                printf("Saliendo del sistema...\n");
                return 0;
            }
        }

    } while (tipoUsuario == 0);

    if (tipoUsuario == 1)
    {
        printf("\nBienvenido/a, %s\n", nombreLogueado);
        menuEmpleado();
    }
    else if (tipoUsuario == 2)
    {
        if (obtenerIdSocio(nombreLogueado, idSocio))
        {
            menuSocio(nombreLogueado, idSocio);
        }
        else
        {
            printf("No se pudieron recuperar los datos del socio.\n");
        }
    }

    return 0;
}
