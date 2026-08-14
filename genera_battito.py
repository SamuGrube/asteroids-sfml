import wave
import struct
import math
import random
import os

SAMPLE_RATE = 44100

def generate_organic_heart_sound(filename, is_s1=True):
    os.makedirs('assets', exist_ok=True)
    filepath = os.path.join('assets', filename)
    samples = []
    
    # S1 (Lub): ~0.16s con micro-doppio impulso (valvola mitrale + tricuspide)
    # S2 (Dub): ~0.12s più secco e a frequenza leggermente più alta (valvola aortica)
    duration = 0.16 if is_s1 else 0.12
    num_samples = int(SAMPLE_RATE * duration)
    
    f_base = 35.0 if is_s1 else 52.0
    f_res = 20.0 if is_s1 else 28.0
    
    # Stato del filtro passa-basso per simulare l'attenuazione dei tessuti toracici
    lpf_state = 0.0
    alpha = 0.06  # Taglia drasticamente le frequenze alte per un suono cupo e toracico

    for i in range(num_samples):
        t = i / SAMPLE_RATE
        
        # 1. Modellazione temporale dei due micro-impulsi valvolari
        if is_s1:
            env1 = math.exp(-32.0 * t) * (math.sin(t * math.pi / 0.02) if t < 0.02 else 1.0)
            t2 = t - 0.022  # Ritardo chiusura seconda valvola (~22ms)
            env2 = (math.exp(-38.0 * t2) * 0.65) if t2 > 0 else 0.0
            env = max(0.0, env1 + env2)
        else:
            env = math.exp(-48.0 * t) * (math.sin(t * math.pi / 0.015) if t < 0.015 else 1.0)

        # 2. Oscillazione di cavità e risonanza muscolare (Sub-bass)
        wave_main = math.sin(2.0 * math.pi * f_base * t)
        wave_sub = math.sin(2.0 * math.pi * f_res * t) * 0.4
        
        # 3. Micro-frizione dei tessuti interni
        muscle_friction = random.uniform(-0.12, 0.12)
        
        raw_signal = (wave_main + wave_sub + muscle_friction) * env
        
        # 4. Applicazione del Low-Pass Filter (LPF)
        lpf_state = lpf_state + alpha * (raw_signal - lpf_state)
        
        samples.append(lpf_state * 2.0)

    with wave.open(filepath, 'w') as f:
        f.setnchannels(1)
        f.setsampwidth(2)
        f.setframerate(SAMPLE_RATE)
        for s in samples:
            s = max(-1.0, min(1.0, s))  # Clamping anti-distorsione
            f.writeframesraw(struct.pack('<h', int(s * 32767)))
    print(f"Generato battito cardiaco organico: {filepath}")

if __name__ == '__main__':
    print("--- Generazione Battito Fisiologico Organico ---")
    generate_organic_heart_sound('beat1.wav', is_s1=True)  # Lub (S1)
    generate_organic_heart_sound('beat2.wav', is_s1=False) # Dub (S2)
    print("--- Fatto! ---")