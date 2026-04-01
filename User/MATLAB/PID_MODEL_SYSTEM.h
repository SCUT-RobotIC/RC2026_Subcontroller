/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: PID_MODEL_SYSTEM.h
 *
 * Code generated for Simulink model 'PID_MODEL_SYSTEM'.
 *
 * Model version                  : 1.10
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Sun Oct 19 18:45:25 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef PID_MODEL_SYSTEM_h_
#define PID_MODEL_SYSTEM_h_
#ifndef PID_MODEL_SYSTEM_COMMON_INCLUDES_
#define PID_MODEL_SYSTEM_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "math.h"
#endif                                 /* PID_MODEL_SYSTEM_COMMON_INCLUDES_ */

#ifndef DEFINED_TYPEDEF_FOR_motor_para_bus_
#define DEFINED_TYPEDEF_FOR_motor_para_bus_

typedef struct {
  real32_T spd_kp[24];
  real32_T spd_ki[24];
  real32_T spd_kd[24];
  real32_T spd_deadband[24];
  real32_T ang_a_kp[24];
  real32_T ang_a_ki[24];
  real32_T ang_a_kd[24];
  real32_T ang_a_deadband[24];
  real32_T ang_s_kp[24];
  real32_T ang_s_ki[24];
  real32_T ang_s_kd[24];
  real32_T ang_s_deadband[24];
} motor_para_bus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_motor_in_
#define DEFINED_TYPEDEF_FOR_motor_in_

typedef struct {
  real32_T mode;
  real32_T target;
  real32_T speed_in;
  real32_T circle_in;
  real32_T ecd;
  real32_T reset;
} motor_in;

#endif

#ifndef DEFINED_TYPEDEF_FOR_motor_out_
#define DEFINED_TYPEDEF_FOR_motor_out_

typedef struct {
  real32_T speed_out;
  real32_T angle_out;
} motor_out;

#endif

/* Block signals and states (default storage) for system '<Root>/For Each Subsystem' */
typedef struct {
  real32_T Saturation;                 /* '<S97>/Saturation' */
  real32_T Saturation_i;               /* '<S45>/Saturation' */
  real32_T Integrator_DSTATE;          /* '<S140>/Integrator' */
  real32_T Filter_DSTATE;              /* '<S135>/Filter' */
  real32_T Filter_DSTATE_n;            /* '<S85>/Filter' */
  real32_T Integrator_DSTATE_h;        /* '<S90>/Integrator' */
  real32_T Integrator_DSTATE_p;        /* '<S38>/Integrator' */
  real32_T Filter_DSTATE_a;            /* '<S33>/Filter' */
  int8_T Filter_PrevResetState;        /* '<S85>/Filter' */
  int8_T Integrator_PrevResetState;    /* '<S90>/Integrator' */
  int8_T Integrator_PrevResetState_d;  /* '<S38>/Integrator' */
  int8_T Filter_PrevResetState_a;      /* '<S33>/Filter' */
} DW_CoreSubsys;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  DW_CoreSubsys CoreSubsys[24];        /* '<Root>/For Each Subsystem' */
  motor_out ImpAsg_InsertedFor_motor_ou[24];/* '<S1>/Bus Creator' */
} DW;

/* External inputs (root inport signals with default storage) */
typedef struct {
  motor_in motor_in_c[24];             /* '<Root>/motor_in' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  motor_out motor_out_c[24];           /* '<Root>/motor_out' */
} ExtY;

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;

/*
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern motor_para_bus motor_para;      /* Variable: motor_para
                                        * Referenced by: '<Root>/For Each Subsystem'
                                        */

/* Model entry point functions */
extern void PID_MODEL_SYSTEM_initialize(void);
extern void PID_MODEL_SYSTEM_step(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S3>/Multiply' : Eliminated nontunable gain of 1
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'PID_MODEL_SYSTEM'
 * '<S1>'   : 'PID_MODEL_SYSTEM/For Each Subsystem'
 * '<S2>'   : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem'
 * '<S3>'   : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1'
 * '<S4>'   : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1'
 * '<S5>'   : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Anti-windup'
 * '<S6>'   : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/D Gain'
 * '<S7>'   : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/External Derivative'
 * '<S8>'   : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Filter'
 * '<S9>'   : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Filter ICs'
 * '<S10>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/I Gain'
 * '<S11>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Ideal P Gain'
 * '<S12>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Ideal P Gain Fdbk'
 * '<S13>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Integrator'
 * '<S14>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Integrator ICs'
 * '<S15>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/N Copy'
 * '<S16>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/N Gain'
 * '<S17>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/P Copy'
 * '<S18>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Parallel P Gain'
 * '<S19>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Reset Signal'
 * '<S20>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Saturation'
 * '<S21>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Saturation Fdbk'
 * '<S22>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Sum'
 * '<S23>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Sum Fdbk'
 * '<S24>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Tracking Mode'
 * '<S25>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Tracking Mode Sum'
 * '<S26>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Tsamp - Integral'
 * '<S27>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Tsamp - Ngain'
 * '<S28>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/postSat Signal'
 * '<S29>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/preSat Signal'
 * '<S30>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Anti-windup/Back Calculation'
 * '<S31>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/D Gain/Internal Parameters'
 * '<S32>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/External Derivative/Error'
 * '<S33>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Filter/Disc. Forward Euler Filter'
 * '<S34>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Filter ICs/Internal IC - Filter'
 * '<S35>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/I Gain/Internal Parameters'
 * '<S36>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Ideal P Gain/Passthrough'
 * '<S37>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Ideal P Gain Fdbk/Disabled'
 * '<S38>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Integrator/Discrete'
 * '<S39>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Integrator ICs/Internal IC'
 * '<S40>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/N Copy/Disabled'
 * '<S41>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/N Gain/Internal Parameters'
 * '<S42>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/P Copy/Disabled'
 * '<S43>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Parallel P Gain/Internal Parameters'
 * '<S44>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Reset Signal/External Reset'
 * '<S45>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Saturation/Enabled'
 * '<S46>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Saturation Fdbk/Disabled'
 * '<S47>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Sum/Sum_PID'
 * '<S48>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Sum Fdbk/Disabled'
 * '<S49>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Tracking Mode/Disabled'
 * '<S50>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Tracking Mode Sum/Passthrough'
 * '<S51>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Tsamp - Integral/TsSignalSpecification'
 * '<S52>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/Tsamp - Ngain/Passthrough'
 * '<S53>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/postSat Signal/Forward_Path'
 * '<S54>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem/Discrete PID Controller1/preSat Signal/Forward_Path'
 * '<S55>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller'
 * '<S56>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1'
 * '<S57>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Anti-windup'
 * '<S58>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/D Gain'
 * '<S59>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/External Derivative'
 * '<S60>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Filter'
 * '<S61>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Filter ICs'
 * '<S62>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/I Gain'
 * '<S63>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Ideal P Gain'
 * '<S64>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Ideal P Gain Fdbk'
 * '<S65>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Integrator'
 * '<S66>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Integrator ICs'
 * '<S67>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/N Copy'
 * '<S68>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/N Gain'
 * '<S69>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/P Copy'
 * '<S70>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Parallel P Gain'
 * '<S71>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Reset Signal'
 * '<S72>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Saturation'
 * '<S73>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Saturation Fdbk'
 * '<S74>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Sum'
 * '<S75>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Sum Fdbk'
 * '<S76>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Tracking Mode'
 * '<S77>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Tracking Mode Sum'
 * '<S78>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Tsamp - Integral'
 * '<S79>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Tsamp - Ngain'
 * '<S80>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/postSat Signal'
 * '<S81>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/preSat Signal'
 * '<S82>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Anti-windup/Passthrough'
 * '<S83>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/D Gain/Internal Parameters'
 * '<S84>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/External Derivative/Error'
 * '<S85>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Filter/Disc. Forward Euler Filter'
 * '<S86>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Filter ICs/Internal IC - Filter'
 * '<S87>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/I Gain/Internal Parameters'
 * '<S88>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Ideal P Gain/Passthrough'
 * '<S89>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S90>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Integrator/Discrete'
 * '<S91>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Integrator ICs/Internal IC'
 * '<S92>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/N Copy/Disabled'
 * '<S93>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/N Gain/Internal Parameters'
 * '<S94>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/P Copy/Disabled'
 * '<S95>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Parallel P Gain/Internal Parameters'
 * '<S96>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Reset Signal/External Reset'
 * '<S97>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Saturation/Enabled'
 * '<S98>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Saturation Fdbk/Disabled'
 * '<S99>'  : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Sum/Sum_PID'
 * '<S100>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Sum Fdbk/Disabled'
 * '<S101>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Tracking Mode/Disabled'
 * '<S102>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Tracking Mode Sum/Passthrough'
 * '<S103>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S104>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/Tsamp - Ngain/Passthrough'
 * '<S105>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/postSat Signal/Forward_Path'
 * '<S106>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller/preSat Signal/Forward_Path'
 * '<S107>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Anti-windup'
 * '<S108>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/D Gain'
 * '<S109>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/External Derivative'
 * '<S110>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Filter'
 * '<S111>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Filter ICs'
 * '<S112>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/I Gain'
 * '<S113>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Ideal P Gain'
 * '<S114>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Ideal P Gain Fdbk'
 * '<S115>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Integrator'
 * '<S116>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Integrator ICs'
 * '<S117>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/N Copy'
 * '<S118>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/N Gain'
 * '<S119>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/P Copy'
 * '<S120>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Parallel P Gain'
 * '<S121>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Reset Signal'
 * '<S122>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Saturation'
 * '<S123>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Saturation Fdbk'
 * '<S124>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Sum'
 * '<S125>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Sum Fdbk'
 * '<S126>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Tracking Mode'
 * '<S127>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Tracking Mode Sum'
 * '<S128>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Tsamp - Integral'
 * '<S129>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Tsamp - Ngain'
 * '<S130>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/postSat Signal'
 * '<S131>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/preSat Signal'
 * '<S132>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Anti-windup/Passthrough'
 * '<S133>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/D Gain/Internal Parameters'
 * '<S134>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/External Derivative/Error'
 * '<S135>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Filter/Disc. Forward Euler Filter'
 * '<S136>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Filter ICs/Internal IC - Filter'
 * '<S137>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/I Gain/Internal Parameters'
 * '<S138>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Ideal P Gain/Passthrough'
 * '<S139>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Ideal P Gain Fdbk/Disabled'
 * '<S140>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Integrator/Discrete'
 * '<S141>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Integrator ICs/Internal IC'
 * '<S142>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/N Copy/Disabled'
 * '<S143>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/N Gain/Internal Parameters'
 * '<S144>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/P Copy/Disabled'
 * '<S145>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Parallel P Gain/Internal Parameters'
 * '<S146>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Reset Signal/Disabled'
 * '<S147>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Saturation/Enabled'
 * '<S148>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Saturation Fdbk/Disabled'
 * '<S149>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Sum/Sum_PID'
 * '<S150>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Sum Fdbk/Disabled'
 * '<S151>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Tracking Mode/Disabled'
 * '<S152>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Tracking Mode Sum/Passthrough'
 * '<S153>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Tsamp - Integral/TsSignalSpecification'
 * '<S154>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/Tsamp - Ngain/Passthrough'
 * '<S155>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/postSat Signal/Forward_Path'
 * '<S156>' : 'PID_MODEL_SYSTEM/For Each Subsystem/If Action_speed Subsystem1/Discrete PID Controller1/preSat Signal/Forward_Path'
 */
#endif                                 /* PID_MODEL_SYSTEM_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
