** si4703 RDS-Tuner-Eval-Board
 * Diese Bibliothek bietet Komfortfunktionen die eine umfänliche Bedienung
 * mit RDS ohne Datenblatt und Bitverschieberei ermoeglicht
 *  + void si4703_init(i2c_bus, i2c_sda, i2c_scl, reset, gp2, ahizen_enable)
 *  + void si4703_germanDefault_setup(callbackPS, callbackRDS, callbackSTC)
 * muessen aufgerufen sein damit
 *  + void si4703_seeking(bool up)
 *  + void si4703_tuning(float_t mhz)
 *  + void si4703_tuneSpace(uint8_t steps, bool up)
 *  + void si4703_stepVolume(bool up)
 *  + void si4703_setSeekQuality(uint8_t quality)-Funktion.
 * 		quality=0=viele Stationen, 1=gute Qualitaet(default), 2=bessere Qualitaet
 * genutzt werden koennen
 * germanDefault_setup wird von 0 bis 2 folgend drei callback-Funktionen als Parameter übergeben
 * welche jeweils mit den nun folgenden Parametern in Nutzercode definiert sein muesssen.
 * 		0: (char[11] text, uint8_t flag) // der acht-stellige Sendername
 * 		1: (char[64] text, uint8_t flag) // der 64-stellige RDS-Text
 *  	2: (float_t mhz, uint8_t rssi, uint8_t flags)// das Statusregister
 * Die Flags sind unten, ueber der Funktion dokumentiert. Die beiden ersten Callbacks werden
 * wiederholt nach vollstaendigem Datenempfang ausgefuehrt, die letztere nach aufruf der folgenden
 * 		void si4703_seeking(bool up)
 * 		void si4703_tuning(float_t mhz)// mit Bandueberschreitung
 * 		void si4703_tuneSpace(uint8_t steps, bool up)// mit Loop über das Band-Ende/Anfang
 * Funktionen
 * Auch alle Register als einfache namentliche Setter-Funktionen mit Bitverschieberei sind geschrieben
 * und ausfuehrlich nach Datenblatt eindokumentiert so das eine einfache Nutzung gegeben ist.
 * Nach den settern muss "void si4703_write_register(uint8_t cnt)"// cnt=10 schreibt alle Control-Register
 * aufgerufen werden
