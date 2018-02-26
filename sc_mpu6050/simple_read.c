#include "/usr/include/stdio.h" //<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "sysfs_helper.h"

static pthread_t agread_id = NULL;

static char *dev_dir_name, *buf_dir_name;
static bool reader_runnning = false;
static bool ag_pass = false;
static int ax, ay, az;
static int gx, gy, gz;
static int cax, cay, caz;
static int cgx, cgy, cgz;
static float fin_anglvel_scale, fin_accel_scale;
static float fx, fy, fz;
static float fgx, fgy, fgz;

const char *iio_dir = "/sys/bus/iio/devices/";

unsigned long timedelay = 100000;
unsigned long buf_len = 128;

int ret, c, i;
int fp;
int err;


int num_channels;
char *trigger_name = NULL;

int datardytrigger = 1;
char *data;
int read_size;
int dev_num, trig_num;
char *buffer_access;
int scan_size;
int noevents = 0;
int p_event = 0, nodmp = 0;
char *dummy;
char chip_name[10];
char device_name[10];
char sysfs[100];


int read_sysfs_posint(char *filename, char *basedir, int* val) {
    int ret = 0;
    int len = 0;
    FILE *sysfsfp;
    char buff[20] = {0};
    char *endptr;
    int filedesc;
    long newval = 0;

    if(val != NULL){
		char *temp = malloc(strlen(basedir) + strlen(filename) + 2);
		if (temp == NULL) {
			DEBUG_MSG("Memory allocation failed");
			return -ENOMEM;
		}
		sprintf(temp, "%s/%s", basedir, filename);
		sysfsfp = fopen(temp, "r");
		if (sysfsfp == NULL) {
			ret = -errno;
			goto error_free;
		}

		filedesc = open( temp, O_RDONLY );
		len = 20;
		len = read( filedesc, buff, len ); /* there was data to read */
	    len=len;
		close( filedesc );
		newval = strtol(buff, &endptr, 10);
		*val = (int)newval;
		fclose(sysfsfp);
		error_free: free(temp);
    }
    return ret;
}

int read_sysfs_float(char *filename, char *basedir, float *val) {
    int ret = 0;
    int len = 0;
    FILE *sysfsfp;
    char *temp = malloc(strlen(basedir) + strlen(filename) + 2);
    if (temp == NULL) {
        DEBUG_MSG("Memory allocation failed");
        return -ENOMEM;
    }
    sprintf(temp, "%s/%s", basedir, filename);
    sysfsfp = fopen(temp, "r");
    if (sysfsfp == NULL) {
        ret = -errno;
        goto error_free;
    }
    len = fscanf(sysfsfp, "%f\n", val);
    len=len;
    fclose(sysfsfp);
    error_free: free(temp);
    return ret;
}

int read_sysfs_string(char *filename, char *basedir, char* val) {
    int ret = 0;
    int len = 0;
    FILE *sysfsfp;

    if(val != NULL){
		char *temp = malloc(strlen(basedir) + strlen(filename) + 2);
		if (temp == NULL) {
			DEBUG_MSG("Memory allocation failed");
			return -ENOMEM;
		}
		sprintf(temp, "%s/%s", basedir, filename);
		sysfsfp = fopen(temp, "r");
		if (sysfsfp == NULL) {
			ret = -errno;
			goto error_free;
		}
		len = fscanf(sysfsfp, "%s\n", val);
	    len=len;
		fclose(sysfsfp);
		error_free: free(temp);
    }
    return ret;
}

/**************************************************************************
 * Function Name  : read_thread
 * Parameter      : none
 * Description    : Thread to supply gyro and accel data over a socket
 * Return         : None
 * Scope          : GLOBAL
 * Comment        : -
 **************************************************************************/
void* ag_read_thread(void* arg) {
	int loop_count = 0;
	read_sysfs_float("in_anglvel_scale", dev_dir_name, &fin_anglvel_scale);
    read_sysfs_float("in_accel_scale", dev_dir_name, &fin_accel_scale);

    //Calibration initialisation
    cax = 0;
    cay = 0;
    caz = 0;
    cgx = 0;
    cgy = 0;
    cgz = 0;

    {
        //Put the column headers up...
        DEBUG_MSG( "  gz    gx    gy      fgz     fgx     fgy         az    ax    ay       faz     fax     fay\n");

        while ( (loop_count < 20) && (reader_runnning) ) {
            //for (j = 0; j < 10000; j++) {
        	//We need to detect a failure so set to pass to start
        	ag_pass = true;
        	if (read_sysfs_posint("in_accel_x_raw", dev_dir_name, &ax) != 0 )
        		ag_pass = false;
        	if (read_sysfs_posint("in_accel_y_raw", dev_dir_name, &ay) != 0 )
        		ag_pass = false;
        	if (read_sysfs_posint("in_accel_z_raw", dev_dir_name, &az) != 0 )
        		ag_pass = false;

        	if (read_sysfs_posint("in_anglvel_x_raw", dev_dir_name, &gx) != 0 )
        		ag_pass = false;
        	if (read_sysfs_posint("in_anglvel_y_raw", dev_dir_name, &gy) != 0 )
        		ag_pass = false;
        	if (read_sysfs_posint("in_anglvel_z_raw", dev_dir_name, &gz) != 0 )
        		ag_pass = false;

    		//We need to exit out and fail.. no point continuing
        	if(ag_pass == false)
        		goto error_out;

#if defined __COMPLIMENTARY_FILTER__
            accData[0] = ax;
            accData[1] = ay;
            accData[2] = az;
            gyrData[0] = gx;
            gyrData[1] = gy;
            gyrData[2] = gz;

            ComplementaryFilter(accData, gyrData, &pitch, &roll);
            //DEBUG_MSG( "pitch %7.2f roll %7.2f \n", pitch, roll);
#endif

            if (abs(gx) < 10) {
                gx = 0;
            }

            if (abs(gy) < 10) {
                gy = 0;
            }

            if (abs(gz) < 10) {
                gz = 0;
            }

            if (abs(ax) < 10) {
                ax = 0;
            }

            if (abs(ay) < 10) {
                ay = 0;
            }

            if (abs(az) < 10) {
                az = 0;
            }

            fx = ax * fin_accel_scale;
            fy = ay * fin_accel_scale;
            fz = az * fin_accel_scale;
            fgx = gx * fin_anglvel_scale;
            fgy = gy * fin_anglvel_scale;
            fgz = gz * fin_anglvel_scale;

            //Convert from radians to degrees......
            fgx *= 57.29577951307855f;
            fgy *= 57.29577951307855f;
            fgz *= 57.29577951307855f;

            	DEBUG_MSG( "%05d %05d %05d  %7.2f %7.2f %7.2f      %05d %05d %05d  %7.2f %7.2f %7.2f\n", gz, gx, gy, fgz, fgx, fgy, az, ax, ay, fz, fx, fy);
            //If required do a simple calibration routine for

 			//Scale as the result is deg/s and our timebase is 100ms (10 times a second)
		    fgy /= 100.0f;
		    fgx /= 100.0f;
		    fgz /= 100.0f;

		    loop_count++;
            //DEBUG_MSG("%7.2f, %7.2f, %7.2f\n", fz, fx, fy);

            //10ms sample rate
            usleep(10000);
        }
    }
error_out:
    reader_runnning = false;

    pthread_exit(NULL);
}





int discover (void)
{
	int ret = 0;
	int asret = 0;

//    DEBUG_MSG("Getting sysfs path\n");
    if (sc_get_sysfs_path(sysfs) != SC_SUCCESS) {
        DEBUG_MSG("get sysfs path fail\n");
        ret = -ENODEV;
        goto error_ret;
    }
//    DEBUG_MSG("sss:::%s\n", sysfs);
    if (sc_get_chip_name(chip_name) != SC_SUCCESS) {
        DEBUG_MSG("get chip name fail\n");
        ret = -ENODEV;
        goto error_ret;
    }
    DEBUG_MSG("chip_name=%s\n", chip_name);

    for (i = 0; i < strlen(chip_name); i++) {
        device_name[i] = tolower(chip_name[i]);
    }
//    device_name[strlen(chip_name)] = '\0';
//    DEBUG_MSG("device name: %s\n", device_name);

    /* Find the device requested */
    dev_num = find_type_by_name(device_name, "iio:device");
    if (dev_num < 0) {
        DEBUG_MSG("Failed to find the %s\n", device_name);
        ret = -ENODEV;
        goto error_ret;
    }
//    DEBUG_MSG("iio device number being used is %d\n", dev_num);

    asprintf(&dev_dir_name, "%siio:device%d", iio_dir, dev_num);
    if (trigger_name == NULL) {
        /*
         * Build the trigger name. If it is device associated it's
         * name is <device_name>_dev[n] where n matches the device
         * number found above
         */
    	asret = asprintf(&trigger_name, "%s-dev%d", device_name, dev_num);
        if (asret < 0) {
            ret = -ENOMEM;
            goto error_ret;
        }
    }
    /* Verify the trigger exists */
    trig_num = find_type_by_name(trigger_name, "trigger");
    if (trig_num < 0) {
        DEBUG_MSG("Failed to find the trigger %s\n", trigger_name);
        ret = -ENODEV;
        goto error_ret;
    }
//    DEBUG_MSG("iio trigger number being used is %d\n", trig_num);
//    DEBUG_MSG("return val %d\n", ret);

    error_ret: return ret;
}




int main(void)
{
  int err;
  char *b;

  ret = discover();

  if( ret )
	  goto error_exit;

  err = pthread_create(&agread_id, NULL, &ag_read_thread, NULL);
  err = err;

  reader_runnning = true;
  
  while(reader_runnning){
	  usleep(200000);
  }
  
  if (agread_id != 0) {
      pthread_join(agread_id, (void**) &b);  
  }

 error_exit:

  close(fp);

  //We are going to return SUCCESS!!!!
  if(ag_pass){
  	ret = 0;
      DEBUG_MSG("Test SUCCESS\n");
  }
  else{
      DEBUG_MSG("Test error %d\n", ret);
  }

  free(buf_dir_name);
  if (datardytrigger)
	free(trigger_name);
  return ret;
}
