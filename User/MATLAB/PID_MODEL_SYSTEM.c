/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 *
 * File: PID_MODEL_SYSTEM.c
 *
 * Code generated for Simulink model 'PID_MODEL_SYSTEM'.
 *
 * Model version                  : 1.11
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Thu Apr  2 13:16:19 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "PID_MODEL_SYSTEM.h"
#include <math.h>
#include "rtwtypes.h"

/* Exported block parameters */
motor_para_bus motor_para = {
  { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F },

  { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F },

  { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F },

  { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F },

  { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F },

  { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F },

  { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F },

  { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F },

  { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F },

  { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F },

  { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F },

  { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F }
} ;                                    /* Variable: motor_para
                                        * Referenced by: '<Root>/For Each Subsystem'
                                        */

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Model step function */
void PID_MODEL_SYSTEM_step(void)
{
  int32_T ForEach_itr;
  real32_T motor_in_c;
  real32_T motor_para_spd_ki;
  real32_T rtb_FilterCoefficient_b;
  real32_T rtb_FilterCoefficient_i;
  real32_T rtb_Sum_d;
  real32_T rtb_Switch2_k;

  /* Outputs for Iterator SubSystem: '<Root>/For Each Subsystem' incorporates:
   *  ForEach: '<S1>/For Each'
   */
  /* Outport: '<Root>/motor_out' */
  for (ForEach_itr = 0; ForEach_itr < 16; ForEach_itr++) {
    /* SwitchCase: '<S1>/Switch Case' incorporates:
     *  ForEachSliceSelector generated from: '<S1>/motor_in'
     *  Inport: '<Root>/motor_in'
     */
    switch ((int32_T)rtU.motor_in_c[ForEach_itr].mode) {
     case 1:
      /* Outputs for IfAction SubSystem: '<S1>/If Action_speed Subsystem' incorporates:
       *  ActionPort: '<S2>/Action Port'
       */
      /* Sum: '<S2>/Sum' */
      rtb_Switch2_k = rtU.motor_in_c[ForEach_itr].target -
        rtU.motor_in_c[ForEach_itr].speed_in;

      /* Switch: '<S2>/Switch2' incorporates:
       *  Abs: '<S2>/Abs'
       *  Constant: '<S2>/Constant'
       */
      if (!(fabsf(rtb_Switch2_k) > motor_para.spd_deadband[ForEach_itr])) {
        rtb_Switch2_k = 0.0F;
      }

      /* End of Switch: '<S2>/Switch2' */
      /* End of Outputs for SubSystem: '<S1>/If Action_speed Subsystem' */
      motor_in_c = rtU.motor_in_c[ForEach_itr].reset;

      /* Outputs for IfAction SubSystem: '<S1>/If Action_speed Subsystem' incorporates:
       *  ActionPort: '<S2>/Action Port'
       */
      /* DiscreteIntegrator: '<S39>/Integrator' */
      if (((motor_in_c > 0.0F) && (rtDW.CoreSubsys[ForEach_itr].
            Integrator_PrevResetState_d <= 0)) || ((motor_in_c <= 0.0F) &&
           (rtDW.CoreSubsys[ForEach_itr].Integrator_PrevResetState_d == 1))) {
        rtDW.CoreSubsys[ForEach_itr].Integrator_DSTATE_p = 0.0F;
      }

      /* DiscreteIntegrator: '<S34>/Filter' */
      if (((motor_in_c > 0.0F) && (rtDW.CoreSubsys[ForEach_itr].
            Filter_PrevResetState_a <= 0)) || ((motor_in_c <= 0.0F) &&
           (rtDW.CoreSubsys[ForEach_itr].Filter_PrevResetState_a == 1))) {
        rtDW.CoreSubsys[ForEach_itr].Filter_DSTATE_a = 0.0F;
      }

      /* Gain: '<S42>/Filter Coefficient' incorporates:
       *  DiscreteIntegrator: '<S34>/Filter'
       *  Gain: '<S32>/Derivative Gain'
       *  Sum: '<S34>/SumD'
       */
      rtb_FilterCoefficient_b = (motor_para.spd_kd[ForEach_itr] * rtb_Switch2_k
        - rtDW.CoreSubsys[ForEach_itr].Filter_DSTATE_a) * 100.0F;

      /* Sum: '<S48>/Sum' incorporates:
       *  DiscreteIntegrator: '<S39>/Integrator'
       *  Gain: '<S44>/Proportional Gain'
       */
      rtb_Sum_d = (motor_para.spd_kp[ForEach_itr] * rtb_Switch2_k +
                   rtDW.CoreSubsys[ForEach_itr].Integrator_DSTATE_p) +
        rtb_FilterCoefficient_b;

      /* Saturate: '<S46>/Saturation' */
      if (rtb_Sum_d > 16384.0F) {
        /* Saturate: '<S46>/Saturation' */
        rtDW.CoreSubsys[ForEach_itr].Saturation_i = 16384.0F;
      } else if (rtb_Sum_d < -16384.0F) {
        /* Saturate: '<S46>/Saturation' */
        rtDW.CoreSubsys[ForEach_itr].Saturation_i = -16384.0F;
      } else {
        /* Saturate: '<S46>/Saturation' */
        rtDW.CoreSubsys[ForEach_itr].Saturation_i = rtb_Sum_d;
      }

      /* End of Saturate: '<S46>/Saturation' */

      /* Gain: '<S31>/Kb' */
      motor_para_spd_ki = motor_para.spd_ki[ForEach_itr];

      /* Update for DiscreteIntegrator: '<S39>/Integrator' incorporates:
       *  DiscreteIntegrator: '<S34>/Filter'
       *  Gain: '<S31>/Kb'
       *  Gain: '<S36>/Integral Gain'
       *  Sum: '<S31>/SumI2'
       *  Sum: '<S31>/SumI4'
       */
      rtDW.CoreSubsys[ForEach_itr].Integrator_DSTATE_p +=
        ((rtDW.CoreSubsys[ForEach_itr].Saturation_i - rtb_Sum_d) *
         motor_para_spd_ki + motor_para_spd_ki * rtb_Switch2_k) * 0.001F;
      if (motor_in_c > 0.0F) {
        rtDW.CoreSubsys[ForEach_itr].Integrator_PrevResetState_d = 1;
        rtDW.CoreSubsys[ForEach_itr].Filter_PrevResetState_a = 1;
      } else if (motor_in_c < 0.0F) {
        rtDW.CoreSubsys[ForEach_itr].Integrator_PrevResetState_d = -1;
        rtDW.CoreSubsys[ForEach_itr].Filter_PrevResetState_a = -1;
      } else if (motor_in_c == 0.0F) {
        rtDW.CoreSubsys[ForEach_itr].Integrator_PrevResetState_d = 0;
        rtDW.CoreSubsys[ForEach_itr].Filter_PrevResetState_a = 0;
      } else {
        rtDW.CoreSubsys[ForEach_itr].Integrator_PrevResetState_d = 2;
        rtDW.CoreSubsys[ForEach_itr].Filter_PrevResetState_a = 2;
      }

      /* End of Update for DiscreteIntegrator: '<S39>/Integrator' */

      /* Update for DiscreteIntegrator: '<S34>/Filter' */
      rtDW.CoreSubsys[ForEach_itr].Filter_DSTATE_a += 0.001F *
        rtb_FilterCoefficient_b;

      /* End of Outputs for SubSystem: '<S1>/If Action_speed Subsystem' */
      break;

     case 2:
      /* Outputs for IfAction SubSystem: '<S1>/If Action_speed Subsystem1' incorporates:
       *  ActionPort: '<S3>/Action Port'
       */
      /* Sum: '<S3>/Sum' incorporates:
       *  Gain: '<S3>/Gain'
       *  Sum: '<S3>/Sum2'
       */
      rtb_FilterCoefficient_b = rtU.motor_in_c[ForEach_itr].target - (8191.0F *
        rtU.motor_in_c[ForEach_itr].circle_in + rtU.motor_in_c[ForEach_itr].ecd);

      /* Abs: '<S3>/Abs' */
      rtb_Switch2_k = fabsf(rtb_FilterCoefficient_b);

      /* Switch: '<S3>/Switch2' incorporates:
       *  Constant: '<S3>/Constant'
       */
      if (!(rtb_Switch2_k > motor_para.ang_a_deadband[ForEach_itr])) {
        rtb_FilterCoefficient_b = 0.0F;
      }

      /* End of Switch: '<S3>/Switch2' */

      /* Gain: '<S148>/Filter Coefficient' incorporates:
       *  DiscreteIntegrator: '<S140>/Filter'
       *  Gain: '<S138>/Derivative Gain'
       *  Sum: '<S140>/SumD'
       */
      rtb_Sum_d = (motor_para.ang_a_kd[ForEach_itr] * rtb_FilterCoefficient_b -
                   rtDW.CoreSubsys[ForEach_itr].Filter_DSTATE) * 100.0F;

      /* Switch: '<S3>/Switch1' incorporates:
       *  Constant: '<S3>/Constant'
       *  Saturate: '<S152>/Saturation'
       *  Sum: '<S3>/Sum1'
       */
      if (rtb_Switch2_k > motor_para.ang_s_deadband[ForEach_itr]) {
        /* Sum: '<S154>/Sum' incorporates:
         *  DiscreteIntegrator: '<S145>/Integrator'
         *  Gain: '<S150>/Proportional Gain'
         */
        motor_para_spd_ki = (motor_para.ang_a_kp[ForEach_itr] *
                             rtb_FilterCoefficient_b +
                             rtDW.CoreSubsys[ForEach_itr].Integrator_DSTATE) +
          rtb_Sum_d;

        /* Saturate: '<S152>/Saturation' */
        if (motor_para_spd_ki > 16384.0F) {
          motor_para_spd_ki = 16384.0F;
        } else if (motor_para_spd_ki < -16384.0F) {
          motor_para_spd_ki = -16384.0F;
        }

        rtb_Switch2_k = motor_para_spd_ki - rtU.motor_in_c[ForEach_itr].speed_in;
      } else {
        rtb_Switch2_k = 0.0F;
      }

      /* End of Switch: '<S3>/Switch1' */
      /* End of Outputs for SubSystem: '<S1>/If Action_speed Subsystem1' */
      motor_in_c = rtU.motor_in_c[ForEach_itr].reset;

      /* Outputs for IfAction SubSystem: '<S1>/If Action_speed Subsystem1' incorporates:
       *  ActionPort: '<S3>/Action Port'
       */
      /* DiscreteIntegrator: '<S88>/Filter' */
      if (((motor_in_c > 0.0F) && (rtDW.CoreSubsys[ForEach_itr].
            Filter_PrevResetState <= 0)) || ((motor_in_c <= 0.0F) &&
           (rtDW.CoreSubsys[ForEach_itr].Filter_PrevResetState == 1))) {
        rtDW.CoreSubsys[ForEach_itr].Filter_DSTATE_n = 0.0F;
      }

      /* Gain: '<S96>/Filter Coefficient' incorporates:
       *  DiscreteIntegrator: '<S88>/Filter'
       *  Gain: '<S86>/Derivative Gain'
       *  Sum: '<S88>/SumD'
       */
      rtb_FilterCoefficient_i = (motor_para.ang_s_kd[ForEach_itr] *
        rtb_Switch2_k - rtDW.CoreSubsys[ForEach_itr].Filter_DSTATE_n) * 100.0F;

      /* DiscreteIntegrator: '<S93>/Integrator' */
      if (((motor_in_c > 0.0F) && (rtDW.CoreSubsys[ForEach_itr].
            Integrator_PrevResetState <= 0)) || ((motor_in_c <= 0.0F) &&
           (rtDW.CoreSubsys[ForEach_itr].Integrator_PrevResetState == 1))) {
        rtDW.CoreSubsys[ForEach_itr].Integrator_DSTATE_h = 0.0F;
      }

      /* Sum: '<S102>/Sum' incorporates:
       *  DiscreteIntegrator: '<S93>/Integrator'
       *  Gain: '<S98>/Proportional Gain'
       */
      motor_para_spd_ki = (motor_para.ang_s_kp[ForEach_itr] * rtb_Switch2_k +
                           rtDW.CoreSubsys[ForEach_itr].Integrator_DSTATE_h) +
        rtb_FilterCoefficient_i;

      /* Saturate: '<S100>/Saturation' */
      if (motor_para_spd_ki > 16384.0F) {
        /* Saturate: '<S100>/Saturation' */
        rtDW.CoreSubsys[ForEach_itr].Saturation = 16384.0F;
      } else if (motor_para_spd_ki < -16384.0F) {
        /* Saturate: '<S100>/Saturation' */
        rtDW.CoreSubsys[ForEach_itr].Saturation = -16384.0F;
      } else {
        /* Saturate: '<S100>/Saturation' */
        rtDW.CoreSubsys[ForEach_itr].Saturation = motor_para_spd_ki;
      }

      /* End of Saturate: '<S100>/Saturation' */

      /* Update for DiscreteIntegrator: '<S145>/Integrator' incorporates:
       *  Gain: '<S142>/Integral Gain'
       */
      rtDW.CoreSubsys[ForEach_itr].Integrator_DSTATE +=
        motor_para.ang_a_ki[ForEach_itr] * rtb_FilterCoefficient_b * 0.001F;

      /* Update for DiscreteIntegrator: '<S140>/Filter' */
      rtDW.CoreSubsys[ForEach_itr].Filter_DSTATE += 0.001F * rtb_Sum_d;

      /* Update for DiscreteIntegrator: '<S88>/Filter' incorporates:
       *  DiscreteIntegrator: '<S93>/Integrator'
       */
      rtDW.CoreSubsys[ForEach_itr].Filter_DSTATE_n += 0.001F *
        rtb_FilterCoefficient_i;
      if (motor_in_c > 0.0F) {
        rtDW.CoreSubsys[ForEach_itr].Filter_PrevResetState = 1;
        rtDW.CoreSubsys[ForEach_itr].Integrator_PrevResetState = 1;
      } else if (motor_in_c < 0.0F) {
        rtDW.CoreSubsys[ForEach_itr].Filter_PrevResetState = -1;
        rtDW.CoreSubsys[ForEach_itr].Integrator_PrevResetState = -1;
      } else if (motor_in_c == 0.0F) {
        rtDW.CoreSubsys[ForEach_itr].Filter_PrevResetState = 0;
        rtDW.CoreSubsys[ForEach_itr].Integrator_PrevResetState = 0;
      } else {
        rtDW.CoreSubsys[ForEach_itr].Filter_PrevResetState = 2;
        rtDW.CoreSubsys[ForEach_itr].Integrator_PrevResetState = 2;
      }

      /* End of Update for DiscreteIntegrator: '<S88>/Filter' */

      /* Update for DiscreteIntegrator: '<S93>/Integrator' incorporates:
       *  Gain: '<S90>/Integral Gain'
       */
      rtDW.CoreSubsys[ForEach_itr].Integrator_DSTATE_h +=
        motor_para.ang_s_ki[ForEach_itr] * rtb_Switch2_k * 0.001F;

      /* End of Outputs for SubSystem: '<S1>/If Action_speed Subsystem1' */
      break;
    }

    /* End of SwitchCase: '<S1>/Switch Case' */

    /* ForEachSliceAssignment generated from: '<S1>/motor_out' incorporates:
     *  BusCreator: '<S1>/Bus Creator'
     */
    rtDW.ImpAsg_InsertedFor_motor_ou[ForEach_itr].speed_out =
      rtDW.CoreSubsys[ForEach_itr].Saturation_i;
    rtDW.ImpAsg_InsertedFor_motor_ou[ForEach_itr].angle_out =
      rtDW.CoreSubsys[ForEach_itr].Saturation;
    rtY.motor_out_c[ForEach_itr] = rtDW.ImpAsg_InsertedFor_motor_ou[ForEach_itr];
  }

  /* End of Outport: '<Root>/motor_out' */
  /* End of Outputs for SubSystem: '<Root>/For Each Subsystem' */
}

/* Model initialize function */
void PID_MODEL_SYSTEM_initialize(void)
{
  {
    int32_T ForEach_itr;

    /* SystemInitialize for Iterator SubSystem: '<Root>/For Each Subsystem' */
    for (ForEach_itr = 0; ForEach_itr < 16; ForEach_itr++) {
      /* SystemInitialize for IfAction SubSystem: '<S1>/If Action_speed Subsystem' */
      /* InitializeConditions for DiscreteIntegrator: '<S39>/Integrator' */
      rtDW.CoreSubsys[ForEach_itr].Integrator_PrevResetState_d = 2;

      /* InitializeConditions for DiscreteIntegrator: '<S34>/Filter' */
      rtDW.CoreSubsys[ForEach_itr].Filter_PrevResetState_a = 2;

      /* End of SystemInitialize for SubSystem: '<S1>/If Action_speed Subsystem' */

      /* SystemInitialize for IfAction SubSystem: '<S1>/If Action_speed Subsystem1' */
      /* InitializeConditions for DiscreteIntegrator: '<S88>/Filter' */
      rtDW.CoreSubsys[ForEach_itr].Filter_PrevResetState = 2;

      /* InitializeConditions for DiscreteIntegrator: '<S93>/Integrator' */
      rtDW.CoreSubsys[ForEach_itr].Integrator_PrevResetState = 2;

      /* End of SystemInitialize for SubSystem: '<S1>/If Action_speed Subsystem1' */
    }

    /* End of SystemInitialize for SubSystem: '<Root>/For Each Subsystem' */
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
