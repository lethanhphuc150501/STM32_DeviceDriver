
#ifndef TIM_H_
#define TIM_H_

#define SR_UIF		(1U<<0)
#define SR_CC1IF	(1U<<1)

void TIM2_1Hz_init(void);
void TIM2_PA0_output_compare(void);
void TIM3_PA6_input_capture(void);

#endif /* TIM_H_ */
