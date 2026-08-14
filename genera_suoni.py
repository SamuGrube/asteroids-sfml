import wave
import struct
import random
import os

SAMPLE_RATE = 44100

def save_wav(filename, samples):
    os.makedirs('assets', exist_ok=True)
    filepath = os.path.join('assets', filename)
    with wave.open(filepath, 'w') as f:
        f.setnchannels(1)
        f.setsampwidth(2)
        f.setframerate(SAMPLE_RATE)
        for s in samples:
            # Clamping per evitare distorsioni digitali
            s = max(-1.0, min(1.0, s))
            f.writeframesraw(struct.pack('<h', int(s * 32767)))
    print(f"Generato: {filepath}")

def generate_shoot():
    # Il classico "Pew-Pew": Onda quadra con frequenza in picchiata rapida
    samples = []
    duration = 0.15
    num_samples = int(SAMPLE_RATE * duration)
    phase = 0.0
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        # Scivola da 880Hz verso il basso in modo esponenziale
        freq = 880.0 * (0.05 ** (t / duration)) 
        phase += freq / SAMPLE_RATE
        val = 0.5 if (phase % 1.0) < 0.5 else -0.5
        env = 1.0 - (i / num_samples) # Fade out
        samples.append(val * env * 0.4)
    save_wav('shoot.wav', samples)

def generate_explosion():
    # Esplosione: Rumore bianco puro (White Noise) con decadimento
    samples = []
    duration = 0.6
    num_samples = int(SAMPLE_RATE * duration)
    for i in range(num_samples):
        val = random.uniform(-1.0, 1.0)
        # Decadimento esponenziale ripido per l'impatto iniziale
        env = (1.0 - (i / num_samples)) ** 3 
        samples.append(val * env * 0.6)
    save_wav('explosion.wav', samples)

def generate_hit():
    # Hit: Rumore sporco a bassa frequenza (crunch)
    samples = []
    duration = 0.2
    num_samples = int(SAMPLE_RATE * duration)
    phase = 0.0
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        freq = 150.0 - (50.0 * (t / duration))
        phase += freq / SAMPLE_RATE
        sq_val = 0.5 if (phase % 1.0) < 0.5 else -0.5
        noise = random.uniform(-0.5, 0.5)
        mix = (sq_val + noise) * 0.5
        env = 1.0 - (i / num_samples)
        samples.append(mix * env * 0.5)
    save_wav('hit.wav', samples)

def generate_gameover():
    # Game Over: Note decrescenti stile pac-man / platform 8-bit
    samples = []
    duration = 1.5
    num_samples = int(SAMPLE_RATE * duration)
    phase = 0.0
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        # Sequenza di note: Mi4 -> Re#4 -> Re4 -> scivolata in basso
        if t < 0.3: freq = 329.63
        elif t < 0.6: freq = 311.13
        elif t < 0.9: freq = 293.66
        else: freq = 293.66 - 200.0 * ((t - 0.9) / 0.6) 
        
        phase += freq / SAMPLE_RATE
        val = 0.5 if (phase % 1.0) < 0.5 else -0.5
        
        # Dissolvenza nell'ultima frazione
        env = 1.0 if t < 0.9 else max(0.0, 1.0 - ((t - 0.9) / 0.6))
        samples.append(val * env * 0.4)
    save_wav('gameover.wav', samples)

if __name__ == '__main__':
    print("Generazione effetti sonori Arcade 8-bit in corso...")
    generate_shoot()
    generate_explosion()
    generate_hit()
    generate_gameover()
    print("Finito! Puoi avviare il gioco.")