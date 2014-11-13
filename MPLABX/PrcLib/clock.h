/*!
 * @file clock.h
 * @author Kevin De Cuyper
 * @brief Header file pour la configuration de la PLL
 */

#ifndef CLOCK_H
#define	CLOCK_H

#define FCY 40000000UL

/**
 * @brief Configuration de la PLL interne � 40MHz (!! ne pas oublier les config bits
 * _FWDT(FWDTEN_OFF) // on d�sactive le Watchdog
_FOSCSEL(FNOSC_FRC);
// enables clock switching and configure the primary oscillator for a 10MHz crystal
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
_FGS(GSS_OFF & //
	GCP_OFF & //
	GWRP_OFF); //
_FICD(ICS_PGD2); // <-- � changer selon les cas
 *
 */
void pllConfig(void);

#endif	/* CLOCK_H */

