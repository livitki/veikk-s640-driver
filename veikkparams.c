/**
 * Module parameters -- see the README for more details
 * @author Jonathan Lam
 */

#include "veikk.h"

// initialize parms with default values
struct veikk_parms veikk_parms = {
    .orientation = 0,
    .da_x = 0, .da_y = 0, .da_width = 0, .da_height = 0,
    .screen_width = 0, .screen_height = 0,
    .pressure_map = 0
};

// parm ops
static int orientation_set(const char *val, const struct kernel_param *kp) {
    if(strlen(val) != 1
       || val[0] > '3'
       || val[0] < '0') {
        return -EINVAL;
    }

    return param_set_uint(val, kp);
}
static const struct kernel_param_ops orientation_ops = {
    .set = orientation_set,
    .get = param_get_uint,
};

#define MAX_PARM_LEN 10
static int screen_map_set(const char *val, const struct kernel_param *kp) {
    unsigned short i;
    char num_tok[MAX_PARM_LEN], *tok_end;
    const char *tok_start, delim = ',';
    unsigned int *parms[] = {
        &veikk_parms.da_x,
        &veikk_parms.da_y,
        &veikk_parms.da_width,
        &veikk_parms.da_height,
        &veikk_parms.screen_width,
        &veikk_parms.screen_height
    };

    // TODO: add error checking
    // TODO: cleaner array parsing? this is like a messy strtok
    //       i.e., is there a a param_set_uint_array?
    tok_start = val;
    i = 0;
    for(tok_end = val; i < 6; tok_end++) {
        if(*tok_end == delim || *tok_end == '\0') {
            *tok_end = '\0';
            strcpy(num_tok, tok_start);
            kstrtouint(num_tok, 10, parms[i]);
            tok_start = ++tok_end;
            i++;
        }
    }

    if(i != 6)
        return -EINVAL;

    return 0;
}
static int screen_map_get(char *val, const struct kernel_param *kp) {
    return sprintf(val, "%i,%i,%i,%i,%i,%i\n",
                        veikk_parms.da_x,
                        veikk_parms.da_y,
                        veikk_parms.da_width,
                        veikk_parms.da_height,
                        veikk_parms.screen_width,
                        veikk_parms.screen_height);
}
static const struct kernel_param_ops screen_map_ops = {
    .set = screen_map_set,
    .get = screen_map_get
};
static const struct kernel_param_ops pressure_map_ops = {
    .set = param_set_uint,
    .get = param_get_uint
};

// create module parms
module_param_cb(orientation, &orientation_ops, &veikk_parms.orientation, 0660);

// custom ops, so no real pointer needed
module_param_cb(screen_map, &screen_map_ops, NULL, 0660);
//module_param_cb(da_x, &screen_map_ops, &veikk_parms.da_x, 0660);
//module_param_cb(da_y, &screen_map_ops, &veikk_parms.da_y, 0660);
//module_param_cb(da_width, &screen_map_ops, &veikk_parms.da_width, 0660);
//module_param_cb(da_height, &screen_map_ops, &veikk_parms.da_height, 0660);
//module_param_cb(screen_width, &screen_map_ops, &veikk_parms.screen_width, 0660);
//module_param_cb(screen_height, &screen_map_ops, &veikk_parms.screen_height,
//                                                0660);
module_param_cb(pressure_map, &pressure_map_ops, &veikk_parms.pressure_map,
                                                0660);

MODULE_LICENSE("GPL");
