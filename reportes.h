#ifndef REPORTES_H
#define REPORTES_H

/* Top 10 de libros con mas prestamos (titulo y autor incluidos). */
void reporteLibrosMasPrestados(void);

/* Libros con menos movimiento, incluidos los que nunca se prestaron. */
void reporteLibrosMenosPrestados(void);

/* Libro con mas prestamos en un anio. Pide el anio por teclado. */
void reporteBestsellerAnual(void);

/* Ranking de autores y ranking de generos por cantidad de prestamos. */
void reporteRankingAutoresGeneros(void);

/* Socios ordenados por cantidad de prestamos. */
void reporteSociosMasActivos(void);

/* Prestamos no devueltos cuya fecha de vencimiento ya paso. */
void reporteMoras(void);

/* Titulos, ejemplares totales y disponibles, y control de coherencia. */
void reporteEstadoInventario(void);

/* Matriz de prestamos: filas = generos, columnas = meses de un anio. */
void reporteMatrizGeneroMes(void);

/* Menu de reportes */
void menuReportes(void);

#endif
