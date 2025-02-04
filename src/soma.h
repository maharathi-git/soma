#ifndef SOMA_H
#define SOMA_H#ifndef SOMA_H
#define SOMA_H

#include <time.h>

// samvathsaralu
static const char* varsham[] = {
		"Prabhava", "Vibhava", "Shukla", "Pramodoota", "Prajotpati",
		"Angeerasa", "Sreemukha", "Bhaava", "Yuva", "Dhaatu",
		"Eeswara", "Bahudhanya", "Pramadi", "Vikrama", "Vrusha",
		"Chitrabhanu", "Svabhanu", "Tarana", "Pardhiva", "Vyaya",
		"Sarvajittu", "Sarvadhari", "Virodhi", "Vikruti", "Khara",
		"Nandana", "Vijaya", "Jaya", "Manmadha", "Durmukhi",
		"Hevilambi", "Vilambi", "Vikari", "Sharvari", "Plava",
		"Shubhakrutu", "Shobhakrutu", "Krodhi", "Vishvavasu", "Parabhava",
		"Plavanga", "Keelaka", "Soumya", "Sadharana", "Virodhikrutu",
		"Pareedhani", "Pramadeecha", "Ananda", "Rakshasa", "Nala",
		"Pingala", "KaLayukti", "Siddhaardhi", "Raudri", "Durmati",
		"Dundubhi", "Rudhirodgari", "Raktakshi", "Krodhana", "Akshaya"
};

char prasthutha_varsham[20];
char enaka_varsham[20];
char mundu_varsham[20];
int prasthutha_varsha_soochika = 0;  // Track the index

void get_prasthutha_varsham() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int year = tm.tm_year + 1900;

    int base_year = 1867;  // Prabhava year in Gregorian calendar
    int cycle_position = (year - base_year) % 60;
    if (cycle_position < 0) cycle_position += 60;

    prasthutha_varsha_soochika=cycle_position;
    snprintf(enaka_varsham, sizeof(enaka_varsham), "%s", varsham[cycle_position-1]);
    snprintf(prasthutha_varsham, sizeof(prasthutha_varsham), "%s", varsham[cycle_position]);
    snprintf(mundu_varsham, sizeof(mundu_varsham), "%s", varsham[cycle_position+1]);
}

void move_year_up()
{
    snprintf(enaka_varsham, sizeof(enaka_varsham), "%s", varsham[prasthutha_varsha_soochika]);
    snprintf(prasthutha_varsham, sizeof(prasthutha_varsham), "%s", varsham[prasthutha_varsha_soochika+1]);
    snprintf(mundu_varsham, sizeof(mundu_varsham), "%s", varsham[prasthutha_varsha_soochika+2]);
}

void move_year_down(){
    snprintf(enaka_varsham, sizeof(enaka_varsham), "%s", varsham[prasthutha_varsha_soochika]);
    snprintf(prasthutha_varsham, sizeof(prasthutha_varsham), "%s", varsham[prasthutha_varsha_soochika-1]);
    snprintf(mundu_varsham, sizeof(mundu_varsham), "%s", varsham[prasthutha_varsha_soochika-2]);
}
/*void get_prasthutha_varsham() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int year = tm.tm_year + 1900;

    int base_year = 1867;  // Prabhava year in Gregorian calendar
    prasthutha_varsha_soochika = (year - base_year) % 60;
    if (prasthutha_varsha_soochika < 0) prasthutha_varsha_soochika += 60;

    snprintf(prasthutha_varsham, sizeof(prasthutha_varsham), "%s", varsham[prasthutha_varsha_soochika]);

    int prev_index = (prasthutha_varsha_soochika - 1 + 60) % 60;
    int next_index = (prasthutha_varsha_soochika + 1) % 60;

    snprintf(enaka_varsham, sizeof(enaka_varsham), "%s", varsham[prev_index]);
    snprintf(mundu_varsham, sizeof(mundu_varsham), "%s", varsham[next_index]);
}*/

// // masalu
// static const char* masam[] = {
 
// };

// // ruthuvulu
// static const char* ruthuvu[] = {

// };

// // nakhatralu
// static const char* nakshatram[] = {

// };

#define THITHI 30
// thithulu
const char *thithulu[THITHI] = {
    "amasa", "shukla padyami", "shukla vidiya", "shukla thadiya",
    "shukla chavithi", "shukla panchami", "shukla shashti", "shukla sapthami",
    "shukla ashtami", "shukla navami", "shukla dashami", "shukla ekadashi",
    "shukla dwadashi", "shukla thrayodashi", "shukla chathurdashi", "punnami",
    "krushna padyami", "krushna vidiya", "krushna thadiya", "krushna chavithi",
    "krushna panchami", "krushna shashti", "krushna sapthami", "krushna ashtami",
    "krushna navami", "krushna dashami", "krushna ekadashi", "krushna dwadashi",
    "krushna thrayodashi", "krushna chathurdashi"
};

const gchar *image_files[THITHI] = {
    "resource/images/36x36/amasa.png", "resource/images/36x36/shukla_padyami.png", 
    "resource/images/36x36/shukla_thadiya.png", "resource/images/36x36/shukla_vidiya.png", 
    "resource/images/36x36/shukla_chavithi.png", "resource/images/36x36/shukla_panchami.png", 
    "resource/images/36x36/shukla_shashti.png", "resource/images/36x36/shukla_sapthami.png", 
    "resource/images/36x36/shukla_ashtami.png", "resource/images/36x36/shukla_navami.png", 
    "resource/images/36x36/shukla_dashami.png", "resource/images/36x36/shukla_ekadashi.png", 
    "resource/images/36x36/shukla_dwadashi.png", "resource/images/36x36/shukla_thrayodashi.png", 
    "resource/images/36x36/shukla_chathurdashi.png", "resource/images/36x36/punnami.png", 
    "resource/images/36x36/krushna_padyami.png", "resource/images/36x36/krushna_thadiya.png", 
    "resource/images/36x36/krushna_vidiya.png", "resource/images/36x36/krushna_chavithi.png", 
    "resource/images/36x36/krushna_panchami.png", "resource/images/36x36/krushna_shashti.png", 
    "resource/images/36x36/krushna_sapthami.png", "resource/images/36x36/krushna_ashtami.png", 
    "resource/images/36x36/krushna_navami.png", "resource/images/36x36/krushna_dashami.png", 
    "resource/images/36x36/krushna_ekadashi.png", "resource/images/36x36/krushna_dwadashi.png", 
    "resource/images/36x36/krushna_thrayodashi.png", "resource/images/36x36/krushna_chathurdashi.png"
};

GdkPixbuf *pixbuf[THITHI];
gboolean load_moon_phase_images()
{    
    for (int i = 0; i < THITHI; i++) {
        pixbuf[i] = gdk_pixbuf_new_from_file(image_files[i], NULL);
        if (pixbuf[i] == NULL) {
            g_printerr("Failed to load image: %s\n", image_files[i]);
            return FALSE;
        }
    }
    return TRUE;
}






// sooryudu
// muhurtham
// static const char* muhurtham[] = {

// };

// // hora
// static const char* gadiya[] = {

// };



#endif


// "Prabhava", "Vibhava", "Shukla", "Pramodoota", "Prajotpati", "Angeerasa", "Sreemukha", "Bhaava", "Yuva", "Dhaatu", "Eeswara", "Bahudhanya", "Pramadi", "Vikrama", "Vrusha", "Chitrabhanu", "Svabhanu", "Tarana", "Pardhiva", "Vyaya", "Sarvajittu", "Sarvadhari", "Virodhi", "Vikruti", "Khara", "Nandana", "Vijaya", "Jaya", "Manmadha", "Durmukhi", "Hevilambi", "Vilambi", "Vikari", "Sharvari", "Plava", "Shubhakrutu", "Shobhakrutu", "Krodhi", "Vishvavasu", "Parabhava", "Plavanga", "Keelaka", "Soumya", "Sadharana", "Virodhikrutu", "Pareedhani", "Pramadeecha", "Ananda", "Rakshasa", "Nala", "Pingala", "KaLayukti", "Siddhaardhi", "Raudri", "Durmati", "Dundubhi", "Rudhirodgari", "Raktakshi", "Krodhana", "Akshaya"


#define THITHI 30

// samvathsaralu
static const char* varsham[] = {
		"Prabhava", "Vibhava", "Shukla", "Pramodoota", "Prajotpati",
		"Angeerasa", "Sreemukha", "Bhaava", "Yuva", "Dhaatu",
		"Eeswara", "Bahudhanya", "Pramadi", "Vikrama", "Vrusha",
		"Chitrabhanu", "Svabhanu", "Tarana", "Pardhiva", "Vyaya",
		"Sarvajittu", "Sarvadhari", "Virodhi", "Vikruti", "Khara",
		"Nandana", "Vijaya", "Jaya", "Manmadha", "Durmukhi",
		"Hevilambi", "Vilambi", "Vikari", "Sharvari", "Plava",
		"Shubhakrutu", "Shobhakrutu", "Krodhi", "Vishvavasu", "Parabhava",
		"Plavanga", "Keelaka", "Soumya", "Sadharana", "Virodhikrutu",
		"Pareedhani", "Pramadeecha", "Ananda", "Rakshasa", "Nala",
		"Pingala", "KaLayukti", "Siddhaardhi", "Raudri", "Durmati",
		"Dundubhi", "Rudhirodgari", "Raktakshi", "Krodhana", "Akshaya"
};

char enaka_varsham[20];
char prasthutha_varsham[20];
char mundu_varsham[20];
int prasthutha_varsha_soochika = -1;  // Track the index

/*void get_prasthutha_varsham() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int year = tm.tm_year + 1900;

    int base_year = 1867;  // Prabhava year in Gregorian calendar
    int cycle_position = (year - base_year) % 60;
    if (cycle_position < 0) cycle_position += 60;

    snprintf(prasthutha_varsham, sizeof(prasthutha_varsham), "%s", varsham[cycle_position]);
}*/

void get_telugu_year() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int year = tm.tm_year + 1900;

    int base_year = 1867;  // Prabhava year in Gregorian calendar
    prasthutha_varsha_soochika = (year - base_year) % 60;
    if (prasthutha_varsha_soochika < 0) prasthutha_varsha_soochika += 60;

    snprintf(current_varsham, sizeof(current_varsham), "%s", varsham[prasthutha_varsha_soochika]);

    int prev_index = (prasthutha_varsha_soochika - 1 + 60) % 60;
    int next_index = (prasthutha_varsha_soochika + 1) % 60;

    snprintf(previous_varsham, sizeof(previous_varsham), "%s", varsham[prev_index]);
    snprintf(next_varsham, sizeof(next_varsham), "%s", varsham[next_index]);
}

// // masalu
// static const char* masam[] = {
 
// };

// // ruthuvulu
// static const char* ruthuvu[] = {

// };

// // nakhatralu
// static const char* nakshatram[] = {

// };

// thithulu
const char *thithulu[THITHI] = {
    "amasa", "shukla padyami", "shukla vidiya", "shukla thadiya",
    "shukla chavithi", "shukla panchami", "shukla shashti", "shukla sapthami",
    "shukla ashtami", "shukla navami", "shukla dashami", "shukla ekadashi",
    "shukla dwadashi", "shukla thrayodashi", "shukla chathurdashi", "punnami",
    "krushna padyami", "krushna vidiya", "krushna thadiya", "krushna chavithi",
    "krushna panchami", "krushna shashti", "krushna sapthami", "krushna ashtami",
    "krushna navami", "krushna dashami", "krushna ekadashi", "krushna dwadashi",
    "krushna thrayodashi", "krushna chathurdashi"
};

const gchar *image_files[THITHI] = {
    "resource/images/36x36/amasa.png", "resource/images/36x36/shukla_padyami.png", 
    "resource/images/36x36/shukla_thadiya.png", "resource/images/36x36/shukla_vidiya.png", 
    "resource/images/36x36/shukla_chavithi.png", "resource/images/36x36/shukla_panchami.png", 
    "resource/images/36x36/shukla_shashti.png", "resource/images/36x36/shukla_sapthami.png", 
    "resource/images/36x36/shukla_ashtami.png", "resource/images/36x36/shukla_navami.png", 
    "resource/images/36x36/shukla_dashami.png", "resource/images/36x36/shukla_ekadashi.png", 
    "resource/images/36x36/shukla_dwadashi.png", "resource/images/36x36/shukla_thrayodashi.png", 
    "resource/images/36x36/shukla_chathurdashi.png", "resource/images/36x36/punnami.png", 
    "resource/images/36x36/krushna_padyami.png", "resource/images/36x36/krushna_thadiya.png", 
    "resource/images/36x36/krushna_vidiya.png", "resource/images/36x36/krushna_chavithi.png", 
    "resource/images/36x36/krushna_panchami.png", "resource/images/36x36/krushna_shashti.png", 
    "resource/images/36x36/krushna_sapthami.png", "resource/images/36x36/krushna_ashtami.png", 
    "resource/images/36x36/krushna_navami.png", "resource/images/36x36/krushna_dashami.png", 
    "resource/images/36x36/krushna_ekadashi.png", "resource/images/36x36/krushna_dwadashi.png", 
    "resource/images/36x36/krushna_thrayodashi.png", "resource/images/36x36/krushna_chathurdashi.png"
};






// sooryudu
// muhurtham
// static const char* muhurtham[] = {

// };

// // hora
// static const char* gadiya[] = {

// };



#endif


// "Prabhava", "Vibhava", "Shukla", "Pramodoota", "Prajotpati", "Angeerasa", "Sreemukha", "Bhaava", "Yuva", "Dhaatu", "Eeswara", "Bahudhanya", "Pramadi", "Vikrama", "Vrusha", "Chitrabhanu", "Svabhanu", "Tarana", "Pardhiva", "Vyaya", "Sarvajittu", "Sarvadhari", "Virodhi", "Vikruti", "Khara", "Nandana", "Vijaya", "Jaya", "Manmadha", "Durmukhi", "Hevilambi", "Vilambi", "Vikari", "Sharvari", "Plava", "Shubhakrutu", "Shobhakrutu", "Krodhi", "Vishvavasu", "Parabhava", "Plavanga", "Keelaka", "Soumya", "Sadharana", "Virodhikrutu", "Pareedhani", "Pramadeecha", "Ananda", "Rakshasa", "Nala", "Pingala", "KaLayukti", "Siddhaardhi", "Raudri", "Durmati", "Dundubhi", "Rudhirodgari", "Raktakshi", "Krodhana", "Akshaya"
