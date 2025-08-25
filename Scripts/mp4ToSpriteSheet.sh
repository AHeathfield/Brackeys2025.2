#!/bin/bash
# Was made using DeepSeek, none of this is my work

# Display usage information
usage() {
    echo "Usage: $0 <mp4file> [frames_per_second]"
    echo "Creates a PNG sprite sheet from an MP4 video"
    echo ""
    echo "Arguments:"
    echo "  mp4file             Path to the input MP4 video file"
    echo "  frames_per_second   Optional: Number of frames to extract per second (default: 6)"
    echo ""
    echo "Examples:"
    echo "  $0 video.mp4         # Uses default 6 FPS"
    echo "  $0 video.mp4 10      # Extracts 10 frames per second"
    echo "  $0 video.mp4 2       # Extracts 2 frames per second"
}

# Check if help was requested
if [[ "$1" == "-h" ]] || [[ "$1" == "--help" ]]; then
    usage
    exit 0
fi

# Check if a file was provided
if [ $# -eq 0 ]; then
    echo "Error: No input file specified!"
    echo ""
    usage
    exit 1
fi

# Get the input file
input_file="$1"

# Set default FPS or use provided value
fps=${2:-6}

# Check if file exists
if [ ! -f "$input_file" ]; then
    echo "Error: File '$input_file' not found!"
    exit 1
fi

# Check if file is an MP4
if [[ "$input_file" != *.mp4 ]]; then
    echo "Error: File must be an MP4 video!"
    exit 1
fi

# Check if FPS is a valid number
if ! [[ "$fps" =~ ^[0-9]+$ ]] || [ "$fps" -lt 1 ]; then
    echo "Error: Frames per second must be a positive integer!"
    exit 1
fi

# Extract base filename without extension
base_name=$(basename "$input_file" .mp4)

echo "Creating sprite sheet from: $input_file"
echo "Extracting $fps frames per second..."

# Extract frames from video
ffmpeg -i "$input_file" -f image2 -vf fps=fps=$fps img%03d.png

# Calculate optimal grid size for a square layout
frame_count=$(ls img*.png | wc -l)

if [ "$frame_count" -eq 0 ]; then
    echo "Error: No frames were extracted. Check your video file and FPS setting."
    exit 1
fi

echo "Extracted $frame_count frames"

# Calculate rows and columns for a square layout
columns=$(echo "sqrt($frame_count)" | bc)
rows=$(( (frame_count + columns - 1) / columns ))  # Ceiling division

echo "Creating ${columns}x${rows} grid sprite sheet..."

# Create the sprite sheet with the calculated grid
montage img*.png -tile ${columns}x${rows} -geometry +0+0 -background none "${base_name}_sprite_sheet.png"

# Clean up temporary images
rm img*.png

echo "Sprite sheet created: ${base_name}_sprite_sheet.png"
