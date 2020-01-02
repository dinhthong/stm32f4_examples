/*
 * pwm.c
 *
 *  Created on: 5 feb 2017
 *      Author: osannolik
 */

#include "pwm.h"
//#include "utils.h"
#include "math.h"

/* Measurements */
extern uint32_t m_pwm_period;

/* Parameters */

extern TIM_HandleTypeDef TimHandle;

void set_ccxe_ch1(TIM_TypeDef* TIMx, uint32_t ch_p_state, uint32_t ch_n_state) {
  uint32_t tmp = (TIM_CCER_CC1NE | TIM_CCER_CC1E) << TIM_CHANNEL_1;
  TIMx->CCER &= ~tmp;
  TIMx->CCER |= ((ch_n_state | ch_p_state) << TIM_CHANNEL_1);
}

static void set_ccxe_ch2(TIM_TypeDef* TIMx, uint32_t ch_p_state, uint32_t ch_n_state) {
  uint32_t tmp = (TIM_CCER_CC1NE | TIM_CCER_CC1E) << TIM_CHANNEL_2;
  TIMx->CCER &= ~tmp;
  TIMx->CCER |= ((ch_n_state | ch_p_state) << TIM_CHANNEL_2);
}

static void set_ccxe_ch3(TIM_TypeDef* TIMx, uint32_t ch_p_state, uint32_t ch_n_state) {
  uint32_t tmp = (TIM_CCER_CC1NE | TIM_CCER_CC1E) << TIM_CHANNEL_3;
  TIMx->CCER &= ~tmp;
  TIMx->CCER |= ((ch_n_state | ch_p_state) << TIM_CHANNEL_3);
}

void set_oc_mode_ch1(TIM_TypeDef* TIMx, uint32_t OC_mode) {
  TIMx->CCMR1 &= ~TIM_CCMR1_OC1M;
  TIMx->CCMR1 |= OC_mode;
}

static void set_oc_mode_ch2(TIM_TypeDef* TIMx, uint32_t OC_mode) {
  TIMx->CCMR1 &= ~TIM_CCMR1_OC2M;
  TIMx->CCMR1 |= (OC_mode << 8);
}

static void set_oc_mode_ch3(TIM_TypeDef* TIMx, uint32_t OC_mode) {
  TIMx->CCMR2 &= ~TIM_CCMR2_OC3M;
  TIMx->CCMR2 |= OC_mode;
}

void set_oc_mode_ch4(TIM_TypeDef* TIMx, uint32_t OC_mode) {
  TIMx->CCMR2 &= ~TIM_CCMR2_OC4M;
  TIMx->CCMR2 |= (OC_mode << 8);
}

void pwm_disable_sample_trigger(void)
{
  set_oc_mode_ch4(TimHandle.Instance, TIM_OCMODE_PWM1);
  TimHandle.Instance->CCR4 = 0u;
}

/* Sets the time when the sample trigger is triggered.
 * Edge-aligned:      0 - 100, where 0 is positive edge.
 * Center-aligned: -100 - 100, where 0 is center of positive
 *                             waveform, and +/-100 is center of low. */
//void pwm_set_sample_trigger_perc(const float duty)
//{
//  const uint32_t full_period = m_pwm_period - 1;
//  const uint32_t ccr = (uint32_t) (0.01f * fabsf(duty) * (float)full_period);

//  TimHandle.Instance->CCR4 = MIN(MAX(ccr, 1u), full_period);

//#if (PWM_EDGE_ALIGNMENT == PWM_CENTER)
//  uint32_t polarity;
//  if (duty > 0.0f) {
//    polarity = TIM_OCMODE_PWM2;
//  } else {
//    polarity = TIM_OCMODE_PWM1;
//  }
//  set_oc_mode_ch4(TimHandle.Instance, polarity);
//#else
//  set_oc_mode_ch4(TimHandle.Instance, TIM_OCMODE_PWM2);
//#endif
//}

void pwm_set_duty_gate_abc_perc(const float duty)
{
  const uint32_t duty_period = (uint32_t) (0.01f * duty * ((float) m_pwm_period));

  TimHandle.Instance->CCR1 = duty_period;
  TimHandle.Instance->CCR2 = duty_period;
  TimHandle.Instance->CCR3 = duty_period;
}
//float duty_pwm_a, duty_pwm_b, duty_pwm_c;
//void pwm_set_duty_gate_perc(const float duty_a, const float duty_b, const float duty_c)
//{
//  const float max_period = (float) m_pwm_period * 0.01f;
//  duty_pwm_a = duty_a;
//  duty_pwm_b = duty_b;
//  duty_pwm_c = duty_c;
//  TimHandle.Instance->CCR1 = (uint32_t) (duty_a * max_period);
//  TimHandle.Instance->CCR2 = (uint32_t) (duty_b * max_period);
//  TimHandle.Instance->CCR3 = (uint32_t) (duty_c * max_period);
//}

void pwm_set_duty_gate_a_perc(const float duty)
{
  const uint32_t duty_period = (uint32_t) (0.01f * duty * ((float) m_pwm_period));
  TimHandle.Instance->CCR1 = duty_period;
}

void pwm_set_duty_gate_b_perc(const float duty)
{
  const uint32_t duty_period = (uint32_t) (0.01f * duty * ((float) m_pwm_period));
  TimHandle.Instance->CCR2 = duty_period;
}

void pwm_set_duty_gate_c_perc(const float duty)
{
  const uint32_t duty_period = (uint32_t) (0.01f * duty * ((float) m_pwm_period));
  TimHandle.Instance->CCR3 = duty_period;
}

void pwm_update_event(void)
{
  HAL_TIM_GenerateEvent(&TimHandle, TIM_EVENTSOURCE_COM);
}

void pwm_break_event(void)
{
  HAL_TIM_GenerateEvent(&TimHandle, TIM_EVENTSOURCE_BREAK);
}

void pwm_ch3_off(void) {
  set_ccxe_ch3(TimHandle.Instance, TIM_CCx_DISABLE, TIM_CCxN_DISABLE);
}

void pwm_ch3_pwm_afw(void) {
  set_oc_mode_ch3(TimHandle.Instance, TIM_OCMODE_PWM1);
  set_ccxe_ch3(TimHandle.Instance, TIM_CCx_ENABLE, TIM_CCxN_ENABLE);
}

void pwm_ch3_sink(void) {
  set_oc_mode_ch3(TimHandle.Instance, TIM_OCMODE_FORCED_ACTIVE);
  set_ccxe_ch3(TimHandle.Instance, TIM_CCx_DISABLE, TIM_CCxN_ENABLE);
}

void pwm_ch2_off(void) {
  set_ccxe_ch2(TimHandle.Instance, TIM_CCx_DISABLE, TIM_CCxN_DISABLE);
}

void pwm_ch2_pwm_afw(void) {
  set_oc_mode_ch2(TimHandle.Instance, TIM_OCMODE_PWM1);
  set_ccxe_ch2(TimHandle.Instance, TIM_CCx_ENABLE, TIM_CCxN_ENABLE);
}

void pwm_ch2_sink(void) {
  set_oc_mode_ch2(TimHandle.Instance, TIM_OCMODE_FORCED_ACTIVE);
  set_ccxe_ch2(TimHandle.Instance, TIM_CCx_DISABLE, TIM_CCxN_ENABLE);
}

void pwm_ch1_off(void) {
  set_ccxe_ch1(TimHandle.Instance, TIM_CCx_DISABLE, TIM_CCxN_DISABLE);
}

void pwm_ch1_pwm_afw(void) {
  set_oc_mode_ch1(TimHandle.Instance, TIM_OCMODE_PWM1);
  set_ccxe_ch1(TimHandle.Instance, TIM_CCx_ENABLE, TIM_CCxN_ENABLE);
}

void pwm_ch1_sink(void) {
  set_oc_mode_ch1(TimHandle.Instance, TIM_OCMODE_FORCED_ACTIVE);
  set_ccxe_ch1(TimHandle.Instance, TIM_CCx_DISABLE, TIM_CCxN_ENABLE);
}
