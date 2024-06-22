APP_PATH="Undead West.app"

xattr -dr com.apple.quarantine "$APP_PATH"
chmod +x "$APP_PATH"/Contents/MacOS/undeadwest
open "$APP_PATH"
