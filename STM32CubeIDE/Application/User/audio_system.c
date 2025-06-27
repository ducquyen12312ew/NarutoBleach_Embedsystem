#include "audio_system.h"
#include "cmsis_os.h"
#include <math.h>

// =================== ADVANCED MELODY SYSTEM ===================

// Chord definitions - arrays of frequencies
typedef struct {
   AudioNote_t notes[4];  // Max 4 notes per chord
   uint8_t note_count;
   char name[8];
} Chord_t;

// Extended note definitions - more notes for richer melodies
#define NOTE_C3_BASS 130     // C3 (Bass) - 130Hz
#define NOTE_D3_BASS 147     // D3 (Bass) - 147Hz
#define NOTE_E3_BASS 165     // E3 (Bass) - 165Hz
#define NOTE_F3_CUSTOM 174   // F3 - 174Hz
#define NOTE_G3_CUSTOM 196   // G3 - 196Hz
#define NOTE_A3_CUSTOM 220   // A3 - 220Hz
#define NOTE_B3_CUSTOM 247   // B3 - 247Hz
#define NOTE_C4_CUSTOM 261   // C4 - 261Hz
#define NOTE_D4_CUSTOM 293   // D4 - 293Hz
#define NOTE_E4_CUSTOM 329   // E4 - 329Hz
#define NOTE_F4_CUSTOM 349   // F4 - 349Hz
#define NOTE_FS4_CUSTOM 370  // F#4 - 370Hz
#define NOTE_G4_CUSTOM 392   // G4 - 392Hz
#define NOTE_A4_CUSTOM 440   // A4 - 440Hz
#define NOTE_B4_CUSTOM 493   // B4 - 493Hz
#define NOTE_C5_CUSTOM 523   // C5 - 523Hz
#define NOTE_D5_CUSTOM 587   // D5 - 587Hz
#define NOTE_E5_CUSTOM 659   // E5 - 659Hz

// Advanced melody structure with multiple sections
typedef struct {
    uint16_t frequency;  // Tần số (Hz)
    uint16_t duration;   // Thời lượng (ms)
    uint8_t volume;      // Âm lượng (0-100)
    uint8_t is_rest;     // 1 = nghỉ, 0 = chơi note
    char description[32]; // Mô tả cảm xúc
} AdvancedMelodyNote_t;

// Total duration: ~15-18 giây với nhiều biến tấu
static const AdvancedMelodyNote_t extended_melody[] = {
    // ===== INTRO (2.8s) - Khởi đầu quyến rũ với cascade =====
    {NOTE_C5_CUSTOM, 300, 12, 0, "Khoi dau cao tuyet"},     // C5 - Khởi đầu cao tuyệt
    {NOTE_A4_CUSTOM, 250, 11, 0, "Roi xuong nhe"},          // A4 - Rơi xuống nhẹ
    {NOTE_F4_CUSTOM, 200, 10, 0, "Tiep tuc cascade"},       // F4 - Tiếp tục cascade
    {NOTE_D4_CUSTOM, 300, 12, 0, "Den diem an toan"},       // D4 - Đến điểm an toàn
    {0, 250, 0, 1, "Nghi ngan de nghe lai"},                // REST - Nghỉ ngắn để nghe lại
    {NOTE_G4_CUSTOM, 350, 14, 0, "Phan hoi vang lai"},      // G4 - Phản hồi vang lại
    {NOTE_E4_CUSTOM, 400, 13, 0, "On dinh chuyen tiep"},    // E4 - Ổn định chuyển tiếp
    {NOTE_C4_CUSTOM, 300, 11, 0, "Ket thuc intro"},         // C4 - Kết thúc intro
    {0, 350, 0, 1, "Nghi dai chuyen verse"},                // REST - Nghỉ dài chuyển verse

    // ===== VERSE 1 (2.8s) - Câu chuyện bắt đầu =====
    {NOTE_C4_CUSTOM, 350, 12, 0, "Cau chuyen bat dau"},  // C4 - Câu chuyện bắt đầu
    {NOTE_E4_CUSTOM, 300, 13, 0, "Phat trien chu de"},   // E4 - Phát triển chủ đề
    {NOTE_G4_CUSTOM, 250, 15, 0, "Tang cuong do"},       // G4 - Tăng cường độ
    {NOTE_A4_CUSTOM, 300, 14, 0, "Diem cao nhe"},        // A4 - Điểm cao nhẹ
    {NOTE_F4_CUSTOM, 350, 12, 0, "Ha xuong mem mai"},    // F4 - Hạ xuống mềm mại
    {NOTE_D4_CUSTOM, 400, 11, 0, "On dinh lai"},         // D4 - Ổn định lại
    {NOTE_G3_CUSTOM, 350, 10, 0, "Ve lai can ban"},      // G3 - Về lại căn bản
    {0, 200, 0, 1, "Nghi ngan"},                         // REST - Nghỉ ngắn

    // ===== CHORUS (3.2s) - Phần cao trào chính =====
    {NOTE_B4_CUSTOM, 400, 18, 0, "Cao trao chinh"},      // B4 - Cao trào chính
    {NOTE_G4_CUSTOM, 300, 16, 0, "Duy tri nang luong"},  // G4 - Duy trì năng lượng
    {NOTE_E4_CUSTOM, 250, 15, 0, "Xuong nhe"},           // E4 - Xuống nhẹ
    {NOTE_C5_CUSTOM, 350, 20, 0, "Dinh cao cam xuc"},    // C5 - Đỉnh cao cảm xúc
    {NOTE_A4_CUSTOM, 300, 17, 0, "Duy tri do cao"},      // A4 - Duy trì độ cao
    {NOTE_F4_CUSTOM, 400, 14, 0, "Ha dan xuong"},        // F4 - Hạ dần xuống
    {NOTE_D4_CUSTOM, 350, 12, 0, "Tro ve binh tinh"},    // D4 - Trở về bình tĩnh
    {NOTE_G3_CUSTOM, 450, 10, 0, "Ket thuc chorus"},     // G3 - Kết thúc chorus
    {0, 350, 0, 1, "Nghi dai de chuyen tiep"},           // REST - Nghỉ dài để chuyển tiếp

    // ===== VERSE 2 (2.6s) - Biến tấu của verse 1 =====
    {NOTE_A3_CUSTOM, 300, 11, 0, "Verse 2 khac biet"},  // A3 - Verse 2 khác biệt
    {NOTE_C4_CUSTOM, 250, 13, 0, "Nhanh hon"},           // C4 - Nhanh hơn
    {NOTE_E4_CUSTOM, 200, 15, 0, "Gan gui hon"},         // E4 - Gần gũi hơn
    {NOTE_G4_CUSTOM, 250, 16, 0, "Tang toc"},            // G4 - Tăng tốc
    {NOTE_B4_CUSTOM, 300, 17, 0, "Len cao nhanh"},       // B4 - Lên cao nhanh
    {NOTE_A4_CUSTOM, 350, 15, 0, "Giu do cao"},          // A4 - Giữ độ cao
    {NOTE_F4_CUSTOM, 300, 13, 0, "Xuong tham lam"},      // F4 - Xuống thám lam
    {NOTE_D4_CUSTOM, 250, 12, 0, "Ve co ban"},           // D4 - Về cơ bản
    {NOTE_A3_CUSTOM, 400, 10, 0, "Chuan bi bridge"},     // A3 - Chuẩn bị bridge
    {0, 250, 0, 1, "Nghi truoc bridge"},                 // REST - Nghỉ trước bridge

    // ===== BRIDGE (3.5s) - Phần đặc biệt với harmony phức tạp =====
    {NOTE_FS4_CUSTOM, 450, 14, 0, "Harmony dac biet"},   // F#4 - Harmony đặc biệt
    {NOTE_A4_CUSTOM, 300, 16, 0, "Tang che am sac"},     // A4 - Tăng chế âm sắc
    {NOTE_D5_CUSTOM, 350, 19, 0, "Len octave cao"},      // D5 - Lên octave cao
    {NOTE_B4_CUSTOM, 300, 17, 0, "Giu khong khi cao"},   // B4 - Giữ không khí cao
    {NOTE_G4_CUSTOM, 400, 15, 0, "Ha dan ve"},           // G4 - Hạ dần về
    {NOTE_E4_CUSTOM, 350, 13, 0, "Quen thuoc"},          // E4 - Quen thuộc
    {NOTE_C4_CUSTOM, 400, 12, 0, "Tro ve binh thuong"},  // C4 - Trở về bình thường
    {NOTE_A3_CUSTOM, 500, 10, 0, "On dinh cho outro"},   // A3 - Ổn định cho outro
    {0, 400, 0, 1, "Nghi dai truoc outro"},              // REST - Nghỉ dài trước outro

    // ===== OUTRO (2.8s) - Kết thúc hoành tráng =====
    {NOTE_E4_CUSTOM, 400, 15, 0, "Ket thuc bat dau"},    // E4 - Kết thúc bắt đầu
    {NOTE_G4_CUSTOM, 300, 14, 0, "Nho lai chu de"},      // G4 - Nhớ lại chủ đề
    {NOTE_B4_CUSTOM, 350, 16, 0, "Len cao lan cuoi"},    // B4 - Lên cao lần cuối
    {NOTE_C5_CUSTOM, 400, 18, 0, "Dinh cao cuoi"},       // C5 - Đỉnh cao cuối
    {NOTE_A4_CUSTOM, 300, 15, 0, "Ha dan xuong"},        // A4 - Hạ dần xuống
    {NOTE_F4_CUSTOM, 350, 13, 0, "Mem mai hon"},         // F4 - Mềm mại hơn
    {NOTE_D4_CUSTOM, 400, 11, 0, "Ve gan goc"},          // D4 - Về gần gốc
    {NOTE_G3_CUSTOM, 450, 9, 0, "Gan goc"},              // G3 - Gần gốc
    {NOTE_E3_BASS, 600, 8, 0, "Ket thuc hoan hao"},      // E3 - Kết thúc hoàn hảo
    {0, 500, 0, 1, "Nghi dai truoc lap lai"},            // REST - Nghỉ dài trước lặp lại
};

#define EXTENDED_MELODY_LENGTH (sizeof(extended_melody) / sizeof(AdvancedMelodyNote_t))

// Define chords with extended harmonies
static const Chord_t chord_definitions[] = {
   // Major chords
   {{NOTE_C4, NOTE_E4, NOTE_G4, NOTE_REST}, 3, "C"},      // C major
   {{NOTE_D4, NOTE_FS4, NOTE_A4, NOTE_REST}, 3, "D"},     // D major
   {{NOTE_E4_CUSTOM, NOTE_G4_CUSTOM, NOTE_B4_CUSTOM, NOTE_REST}, 3, "Em"},  // Em
   {{NOTE_F4, NOTE_A4, NOTE_C5, NOTE_REST}, 3, "F"},      // F major
   {{NOTE_G4, NOTE_B4, NOTE_D5, NOTE_REST}, 3, "G"},      // G major
   {{NOTE_A4, NOTE_CS5, NOTE_E5, NOTE_REST}, 3, "A"},     // A major
   {{NOTE_B4, NOTE_DS5, NOTE_FS5, NOTE_REST}, 3, "B"},    // B major

   // Minor chords
   {{NOTE_C4, NOTE_DS4, NOTE_G4, NOTE_REST}, 3, "Cm"},    // C minor
   {{NOTE_D4, NOTE_F4, NOTE_A4, NOTE_REST}, 3, "Dm"},     // D minor
   {{NOTE_E4_CUSTOM, NOTE_G4_CUSTOM, NOTE_B4_CUSTOM, NOTE_REST}, 3, "Em2"},  // E minor
   {{NOTE_F4, NOTE_GS4, NOTE_C5, NOTE_REST}, 3, "Fm"},    // F minor
   {{NOTE_G4, NOTE_AS4, NOTE_D5, NOTE_REST}, 3, "Gm"},    // G minor
   {{NOTE_A4, NOTE_C5, NOTE_E5, NOTE_REST}, 3, "Am"},     // A minor

   // Extended chords
   {{NOTE_C4, NOTE_E4, NOTE_G4, NOTE_B4}, 4, "Cmaj7"},    // Cmaj7
   {{NOTE_D4, NOTE_FS4, NOTE_A4, NOTE_C5}, 4, "D7"},      // D7
   {{NOTE_E4_CUSTOM, NOTE_G4_CUSTOM, NOTE_B4_CUSTOM, NOTE_D5_CUSTOM}, 4, "Em7"}, // Em7
};

#define CHORD_COUNT (sizeof(chord_definitions) / sizeof(Chord_t))

// Multiple progression patterns for variety
static const uint8_t progression_epic[] = {0, 5, 2, 4, 1, 12};           // C-A-Em-G-D-Am (epic)
static const uint8_t progression_dreamy[] = {2, 12, 4, 2, 14, 15};       // Em-Am-G-Em-Cmaj7-D7 (dreamy)
static const uint8_t progression_floating[] = {2, 6, 2, 4, 16};          // Em-B-Em-G-Em7 (floating)
static const uint8_t progression_cinematic[] = {14, 15, 16, 2, 4, 0};    // Cmaj7-D7-Em7-Em-G-C (cinematic)

// System variables
static uint8_t chord_playing = 0;
static uint8_t current_chord_index = 0;
static uint8_t current_note_in_chord = 0;
static uint32_t chord_start_time = 0;
static uint32_t note_start_time = 0;
static uint8_t chord_repeat_count = 0;
static uint8_t global_volume = 15;
static AudioState_t audio_state = AUDIO_STATE_IDLE;

// Current progression (now supports longer progressions)
static const uint8_t* current_progression = progression_dreamy;
static uint8_t progression_length = 6;
static uint8_t current_progression_index = 0;

// Enhanced timing settings
#define NOTE_DURATION_MS 200           // Faster for more dynamic feel
#define CHORD_REPEAT_COUNT 6           // Fewer repeats for progression variety
#define CHORD_REST_DURATION_MS 150     // Shorter rest for smoother flow
#define CHORD_TOTAL_DURATION_MS (NOTE_DURATION_MS * 3 * CHORD_REPEAT_COUNT + CHORD_REST_DURATION_MS)

TIM_HandleTypeDef htim13;

// =================== CORE FUNCTIONS ===================

void Audio_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   __HAL_RCC_TIM13_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();

   GPIO_InitStruct.Pin = GPIO_PIN_6;
   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   GPIO_InitStruct.Alternate = GPIO_AF9_TIM13;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   htim13.Instance = TIM13;
   htim13.Init.Prescaler = 21 - 1;
   htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
   htim13.Init.Period = 20000 - 1;
   htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
   htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

   if (HAL_TIM_PWM_Init(&htim13) != HAL_OK) {
       Error_Handler();
   }

   TIM_OC_InitTypeDef sConfigOC = {0};
   sConfigOC.OCMode = TIM_OCMODE_PWM1;
   sConfigOC.Pulse = 1000;
   sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
   sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;

   if (HAL_TIM_PWM_ConfigChannel(&htim13, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
       Error_Handler();
   }
}

/**
* @brief Enhanced note playing with dynamic volume curves
*/
void Audio_PlayNote(AudioNote_t note, uint8_t volume)
{
   if (note == NOTE_REST || volume == 0) {
       HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);
       return;
   }

   uint32_t timer_clock = 8000000;
   uint32_t period = (uint32_t)(timer_clock / (float)note);

   // Dynamic duty cycle based on frequency for better sound quality
   uint32_t duty_cycle;
   if (note < 200) {
       duty_cycle = period / 15; // Bass notes - stronger
   } else if (note < 400) {
       duty_cycle = period / 18; // Mid notes - balanced
   } else {
       duty_cycle = period / 22; // High notes - softer
   }

   // Apply volume with enhanced curve
   duty_cycle = (duty_cycle * volume * global_volume) / (100 * 100);

   // Dynamic limits based on frequency
   uint32_t min_duty = period / 80;
   uint32_t max_duty = period / 8;
   if (duty_cycle < min_duty) duty_cycle = min_duty;
   if (duty_cycle > max_duty) duty_cycle = max_duty;

   if (period < 100) period = 100;

   __HAL_TIM_SET_AUTORELOAD(&htim13, period - 1);
   __HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, duty_cycle);
   HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
}

void Audio_StopNote(void)
{
   HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);
}

// Global variables for extended melody playback
static uint8_t extended_melody_playing = 0;
static uint32_t extended_start_time = 0;
static uint8_t extended_current_note_index = 0;
static uint8_t extended_note_started = 0;

/**
* @brief Start extended melody playback: 6 sections với biến tấu phong phú
*/
void Audio_StartExtendedMelody(void)
{
   extended_melody_playing = 1;
   extended_start_time = HAL_GetTick();
   extended_current_note_index = 0;
   extended_note_started = 0;
   audio_state = AUDIO_STATE_PLAYING_MUSIC;
}

/**
* @brief Stop extended melody
*/
void Audio_StopExtendedMelody(void)
{
   extended_melody_playing = 0;
   audio_state = AUDIO_STATE_IDLE;
   Audio_StopNote();
}

/**
* @brief Set chord progression type với nhiều options hơn
*/
static void Audio_SetProgression(uint8_t prog_type)
{
   switch(prog_type) {
       case 0: // Epic: C-A-Em-G-D-Am
           current_progression = progression_epic;
           progression_length = 6;
           break;
       case 1: // Dreamy: Em-Am-G-Em-Cmaj7-D7
           current_progression = progression_dreamy;
           progression_length = 6;
           break;
       case 2: // Floating: Em-B-Em-G-Em7
           current_progression = progression_floating;
           progression_length = 5;
           break;
       case 3: // Cinematic: Cmaj7-D7-Em7-Em-G-C
           current_progression = progression_cinematic;
           progression_length = 6;
           break;
       default:
           current_progression = progression_dreamy;
           progression_length = 6;
           break;
   }
}

/**
* @brief Stop chord progression
*/
void Audio_StopChordProgression(void)
{
   chord_playing = 0;
   audio_state = AUDIO_STATE_IDLE;
   Audio_StopNote();
}

/**
* @brief Check if should stop audio (call this when changing screen)
*/
void Audio_OnScreenChange(void)
{
   // Dừng tất cả audio khi chuyển screen
   Audio_StopExtendedMelody();
   Audio_StopChordProgression();
}

/**
* @brief Enhanced chord playing with dynamic timing
*/
void Audio_PlayChord(uint8_t chord_index, uint16_t duration_ms, uint8_t volume)
{
   if (chord_index >= CHORD_COUNT) return;

   const Chord_t* chord = &chord_definitions[chord_index];
   uint8_t note_duration = NOTE_DURATION_MS;
   uint16_t total_cycles = duration_ms / (note_duration * chord->note_count + CHORD_REST_DURATION_MS);

   for (uint16_t cycle = 0; cycle < total_cycles; cycle++) {
       for (uint8_t note_idx = 0; note_idx < chord->note_count; note_idx++) {
           // Dynamic volume based on note position in chord
           uint8_t dynamic_volume = volume + (note_idx * 2); // Slight volume increase for higher notes
           if (dynamic_volume > 100) dynamic_volume = 100;

           Audio_PlayNote(chord->notes[note_idx], dynamic_volume);
           HAL_Delay(note_duration);
       }
       // Dynamic rest based on cycle
       Audio_StopNote();
       uint16_t rest_time = CHORD_REST_DURATION_MS + (cycle % 3) * 20; // Varied rest times
       HAL_Delay(rest_time);
   }
   Audio_StopNote();
}

/**
* @brief Multiple test functions for different moods
*/
void Audio_TestEpicSequence(void)
{
   // Epic chord sequence
   Audio_PlayChord(0, 800, 75);   // C major
   HAL_Delay(100);
   Audio_PlayChord(5, 800, 80);   // A major
   HAL_Delay(100);
   Audio_PlayChord(2, 1000, 85);  // Em - longer and louder
   HAL_Delay(200);
}

void Audio_TestDreamySequence(void)
{
   // Dreamy floating sequence
   for (int i = 0; i < 3; i++) {
       uint8_t fade_volume = 60 + (i * 5);
       Audio_PlayNote(NOTE_E4_CUSTOM, fade_volume);
       HAL_Delay(300);
       Audio_PlayNote(NOTE_G4_CUSTOM, fade_volume + 5);
       HAL_Delay(250);
       Audio_PlayNote(NOTE_B4_CUSTOM, fade_volume + 10);
       HAL_Delay(400);
       Audio_PlayNote(NOTE_C5_CUSTOM, fade_volume + 8);
       HAL_Delay(350);
   }
   Audio_StopNote();
}

void Audio_TestCinematicSequence(void)
{
   // Cinematic build-up
   uint16_t durations[] = {400, 350, 300, 250, 200, 150};
   uint8_t volumes[] = {50, 55, 60, 70, 80, 90};
   uint16_t notes[] = {NOTE_E3_BASS, NOTE_G3_CUSTOM, NOTE_B3_CUSTOM,
                       NOTE_E4_CUSTOM, NOTE_G4_CUSTOM, NOTE_B4_CUSTOM};

   for (int i = 0; i < 6; i++) {
       Audio_PlayNote(notes[i], volumes[i]);
       HAL_Delay(durations[i]);
   }
   Audio_StopNote();
   HAL_Delay(500);
}

/**
* @brief Start enhanced chord progression
*/
void Audio_PlayEnhancedProgression(uint8_t prog_type)
{
   Audio_SetProgression(prog_type);
   chord_playing = 1;
   current_progression_index = 0;
   current_chord_index = current_progression[0];
   current_note_in_chord = 0;
   chord_repeat_count = 0;
   chord_start_time = HAL_GetTick();
   note_start_time = HAL_GetTick();
   audio_state = AUDIO_STATE_PLAYING_MUSIC;
}

// Enhanced compatibility functions
void Audio_PlayBackgroundMusic(void) { Audio_StartExtendedMelody(); }     // Extended melody
void Audio_PlayCyberpunkAmbient(void) { Audio_PlayEnhancedProgression(3); } // Cinematic progression
void Audio_TriggerGlitch(void) { Audio_TestEpicSequence(); }              // Epic test
void Audio_StopBackgroundMusic(void) { Audio_StopExtendedMelody(); }
void Audio_StopCyberpunkAmbient(void) { Audio_StopChordProgression(); }

void Audio_SetVolume(uint8_t volume)
{
   if (volume <= 100) {
       global_volume = volume;
   } else {
       global_volume = 100;
   }
}

void Audio_SetDigitalEffects(uint8_t enable) { /* Not used */ }
void Audio_FadeOut(uint16_t fade_time_ms) { Audio_StopExtendedMelody(); }
void Audio_FadeIn(uint16_t fade_time_ms) { Audio_PlayBackgroundMusic(); }

/**
* @brief Enhanced main update loop
*/
void Audio_Update(void)
{
   // Handle extended melody playback (6 sections, ~15-18 giây)
   if (extended_melody_playing) {
       uint32_t current_time = HAL_GetTick();
       uint32_t elapsed_time = current_time - extended_start_time;

       // Calculate cumulative time for current note
       uint32_t cumulative_time = 0;
       uint8_t target_note_index = 0;

       // Find which note should be playing at current elapsed time
       for (uint8_t i = 0; i < EXTENDED_MELODY_LENGTH; i++) {
           if (elapsed_time < cumulative_time + extended_melody[i].duration) {
               target_note_index = i;
               break;
           }
           cumulative_time += extended_melody[i].duration;
           target_note_index = i + 1;
       }

       // Check if we've completed the extended loop
       if (target_note_index >= EXTENDED_MELODY_LENGTH) {
           // Reset for smooth loop with slight variation
           extended_start_time = current_time;
           extended_current_note_index = 0;
           extended_note_started = 0;
           return;
       }

       // Start new note if we've moved to a different note
       if (target_note_index != extended_current_note_index || !extended_note_started) {
           extended_current_note_index = target_note_index;
           extended_note_started = 1;

           const AdvancedMelodyNote_t* current_note = &extended_melody[extended_current_note_index];

           if (current_note->is_rest) {
               // REST - tắt âm thanh
               Audio_StopNote();
           } else {
               // Chơi note với enhanced dynamics
               Audio_PlayNote(current_note->frequency, current_note->volume);
           }
       }

       return; // Skip chord progression when extended melody is playing
   }

   // Enhanced chord progression handling
   if (!chord_playing || audio_state != AUDIO_STATE_PLAYING_MUSIC) {
       return;
   }

   uint32_t current_time = HAL_GetTick();

   // Check if it's time to switch to next note in chord
   if (current_time - note_start_time >= NOTE_DURATION_MS) {
       // Get current chord
       uint8_t chord_idx = current_progression[current_progression_index];
       const Chord_t* current_chord = &chord_definitions[chord_idx];

       // Move to next note in chord
       current_note_in_chord++;
       if (current_note_in_chord >= current_chord->note_count) {
           current_note_in_chord = 0;
           chord_repeat_count++;
       }

       // Check if chord is finished
       if (chord_repeat_count >= CHORD_REPEAT_COUNT) {
           // Move to next chord in progression
           current_progression_index++;
           if (current_progression_index >= progression_length) {
               current_progression_index = 0; // Loop progression
           }
           chord_repeat_count = 0;
           current_note_in_chord = 0;
           chord_start_time = current_time;
       }

       // Play current note with dynamic volume based on progression position
       chord_idx = current_progression[current_progression_index];
       current_chord = &chord_definitions[chord_idx];
       AudioNote_t note_to_play = current_chord->notes[current_note_in_chord];

       // Dynamic volume based on progression position and note position
       uint8_t dynamic_volume = 45 + (current_progression_index * 2) + (current_note_in_chord * 3);
       if (dynamic_volume > 75) dynamic_volume = 75;

       Audio_PlayNote(note_to_play, dynamic_volume);

       note_start_time = current_time;
   }
}
