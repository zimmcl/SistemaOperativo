/*
 * desencriptador.c
 *
 *      Author: Zimmel, Ezequiel
 *				Ceballos, Matias
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>

#define SUCCESS 0
#define Dispositivo "desencriptador"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZIMMEL, Ezequiel | CEBALLOS, Matias");
MODULE_VERSION("1.0.0");

/* Major Number */
static int major_number = 51;


/* Valor restado a cada caracter del mensaje a decodificar */
static int valor = 1;

/* Mensaje a desencriptar */
static char mensaje[100] = {0};

/* Mensaje desencriptado */
static char mensaje_desencriptado[100] = {0};


#define SUCCESS 0
static int Device_Open = 0;

/*
* Funciones
*/

int device_open(struct inode *inode, struct file *file);
ssize_t device_read (struct file *file, char __user *buffer, size_t length, loff_t *offset);
ssize_t device_write (struct file *file, const char __user *buffer, size_t length, loff_t *offset);
int device_close (struct inode *inode, struct file *file);
void desencriptar_mensaje(void);

/*
* Esta estructura ejecutará las funciones que se llamen cuando
* un proceso hace algo al dispositivo que se crea.
*/

struct file_operations fops = {
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release = device_close
};


/* Se llama cuando un proceso intenta abrir el archivo del dispositivo. */
int device_open(struct inode *inode, struct file *file) {
	printk(KERN_INFO "Dispositivo de desencriptamiento abierto\n");

	/* Prohibimos la ejecución de dos procesos al mismo tiempo */
	if (Device_Open)
		return -EBUSY;

	Device_Open++;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

/* Esta función se llama cuando un proceso que ya ha abierto el archivo
 * del dispositivo intenta leer de él.
 * */
ssize_t device_read (struct file *file, char __user *buffer, size_t length, loff_t *offset) {
	int k;
	int bytes_read = 0; /*Número de bytes escritos en el buffer*/

	/*Si estamos al final del mensaje, devuelve 0*/
	if((*offset) >= strlen(mensaje_desencriptado)) {
		return 0;
	}

	/*Coloca los datos en el buffer*/
	for(k = 0; k < strlen(mensaje_desencriptado); k++) {
		put_user(mensaje_desencriptado[k], buffer++);
		bytes_read++;
	}

	(*offset) += bytes_read;

	/*Se retorna el número de bytes insertados en el buffer*/
	return bytes_read;
}

/* Esta función se llama cuando alguien intenta escribir en su archivo de dispositivo. */
ssize_t device_write (struct file *file, const char __user *buffer, size_t length, loff_t *offset) {

	/*Copio 0 en el total de posiciones de mensaje y mensaje_desencriptado*/
	memset(mensaje, 0, 100);
	memset(mensaje_desencriptado, 0, 100);

	strncpy(mensaje, buffer, length);

	desencriptar_mensaje();

	return length;
}

/* Se llama cuando un proceso intenta cerrar el archivo del dispositivo. */
int device_close (struct inode *inode, struct file *file) {
	printk(KERN_INFO "Dispositivo de desencriptamiento cerrado\n");
	Device_Open--;

	module_put(THIS_MODULE);
	return SUCCESS;
}

/*Registrar el dispositivo*/
int driver_init(void)
{
	register_chrdev(major_number, Dispositivo, &fops);
	return SUCCESS;
}

/*Desregistrar el dispositivo*/
void driver_exit(void)
{
	unregister_chrdev(major_number, Dispositivo);
}

/* Funcion que desencripta el mensaje */
void desencriptar_mensaje(void) {
	int k;
	for(k = 0; k < strlen(mensaje); k++) {
		mensaje_desencriptado[k] = mensaje[k] - valor;
	}
}

module_init(driver_init);
module_exit(driver_exit);


