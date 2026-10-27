#include <Arduino.h>

#define BUZZER_PIN    18
#define LEDC_CHANNEL  0
#define TICK_MS       50

#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494

struct Note
{
    uint16_t freq;
    uint16_t duration;
};

const Note melody[] =
{
    {NOTE_E4, 300},
    {NOTE_E4, 300},
    {NOTE_E4, 600},

    {NOTE_E4, 300},
    {NOTE_E4, 300},
    {NOTE_E4, 600},

    {NOTE_E4, 300},
    {NOTE_G4, 300},
    {NOTE_C4, 300},
    {NOTE_D4, 300},
    {NOTE_E4, 800},
};

const uint16_t melodySize =
    sizeof(melody) / sizeof(melody[0]);

class Synth
{
private:
    bool playing = false;

    uint16_t currentNote = 0;

    uint32_t lastTick = 0;
    uint32_t elapsed = 0;

public:

    void begin()
    {
        ledcSetup(LEDC_CHANNEL, 2000, 10);
        ledcAttachPin(BUZZER_PIN, LEDC_CHANNEL);
    }

    void play()
    {
        currentNote = 0;
        elapsed = 0;
        playing = true;

        ledcWriteTone(
            LEDC_CHANNEL,
            melody[currentNote].freq
        );
    }

    void update()
    {
        if (!playing)
            return;

        uint32_t now = millis();

        if (now - lastTick < TICK_MS)
            return;

        lastTick = now;
        elapsed += TICK_MS;

        if (elapsed >= melody[currentNote].duration)
        {
            elapsed = 0;

            currentNote++;

            if (currentNote >= melodySize)
            {
                currentNote = 0; // повтор мелодії
            }

            ledcWriteTone(
                LEDC_CHANNEL,
                melody[currentNote].freq
            );
        }
    }
};

Synth player;

void setup()
{
    player.begin();
    player.play();
}

void loop()
{
    player.update();

    // Тут можуть працювати інші задачі
}