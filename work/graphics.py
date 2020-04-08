import matplotlib.pyplot as plt
import numpy as np
import wave
import sys
from matplotlib.ticker import (AutoMinorLocator, MultipleLocator)

spf = wave.open("seno.wav", "r")
# Extract Raw Audio from Wav File
signal = spf.readframes(-1)
signal = np.fromstring(signal, "Int16")
signal = signal[0:1500]
# If Stereo
if spf.getnchannels() == 2:
    print("Just mono files")
    sys.exit(0)
fig, ax = plt.subplots(figsize=(10, 8))

# Set axis ranges; by default this will put major ticks every 25.
ax.set_xlim(0, 1500)
ax.set_ylim(-14000, 14000)

# Change major ticks to show every 20.
ax.xaxis.set_major_locator(MultipleLocator(150))
ax.yaxis.set_major_locator(MultipleLocator(1000))

# Change minor ticks to show every 5. (20/4 = 5)
ax.xaxis.set_minor_locator(AutoMinorLocator(2))
ax.yaxis.set_minor_locator(AutoMinorLocator(2))

# Turn grid on for both major and minor ticks and style minor slightly
# differently.
ax.grid(which='major', color='#CCCCCC', linestyle='--')
ax.grid(which='minor', color='#CCCCCC', linestyle=':')
plt.plot(signal, 'r-o', markersize = 4, label = "Seno")

spf2 = wave.open("doremi.wav", "r")
# Extract Raw Audio from Wav File
signal2 = spf2.readframes(-1)
signal2 = np.fromstring(signal2, "Int16")
signal2 = signal2[0:1500]
# If Stereo
if spf2.getnchannels() == 2:
    print("Just mono files")
    sys.exit(0)
plt.plot(signal2, 'b-x', markersize = 4, label = "Dumb")
plt.legend()
plt.show()