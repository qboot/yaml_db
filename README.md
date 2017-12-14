# Fresh GTK+3 Install on Mac OS X

*Tested on High Sierra only.*

## Requirements

- XCode > 9.0
- [Homebrew](https://brew.sh/index_fr.html)

## Install GTK+3 and its deps

```console
$ brew update
$ brew install gtk+3
```

## Configure XCode: xcodeproj

### Build Settings

- Click on `yaml_db` file in project tree
- Go to `Build Settings`
- Search for `Search Paths`
- Add all these paths to `Header Search Paths` :
    ```
    /usr/local/opt/gtk+3/include
    /usr/local/opt/glib/include
    /usr/local/opt/cairo/include
    /usr/local/opt/pango/include
    /usr/local/opt/gdk-pixbuf/include
    /usr/local/opt/atk/include
    /usr/local/opt/gtk+3/lib
    /usr/local/opt/glib/lib
    ```

    *Pro-Tips: You can copy/paste them all in one line, then click away of the box to changes take effect*

- Make them all `recursive`
- Add all these paths to `Library Search Paths` :
    ```
    /usr/local/opt/gtk+3/lib
    /usr/local/opt/glib/lib
    ```
- Make them all `recursive` too
- Search for `Documentation Comments`
- Select `No`

### Library Files

- Right click on `libs` directory in project tree
- Click on `Add Files to...`
- Then do `CMD+SHIFT+G` shortcut to open `Go To...` menu
- Type `/usr/local/lib` and press Go
- Select all these library files:
    ```
    libatk-1.0.0.dylib
    libgdk-3.0.dylib
    libgdk_pixbuf-2.0.0.dylib
    libglib-2.0.0.dylib
    libgmodule-2.0.0.dylib
    libgobject-2.0.0.dylib
    libgthread-2.0.0.dylib
    libgtk-3.0.dylib
    libpango-1.0.0.dylib
    ```

- Click on `Add`
- **Try to compile! (`CMD+R`)**

## Useful resources

- A good explanation about why all this is needed: ["Dependencies" section in Gnome GTK+ Documentation](https://developer.gnome.org/gtk3/stable/gtk-building.html)
- French not maintened course on OpenClassrooms: [Créez une interface avec GTK](https://openclassrooms.com/courses/creez-une-interface-avec-gtk)
- Really good and maintened german documentation on GTK+3: [GTK mit Builder](https://de.wikibooks.org/wiki/GTK_mit_Builder) 

## Troubleshooting

**My Build Settings has too many columns**

Make sure you have checked `Combined` display option and not `Levels`.

**I have console warning: Month 13 is out of bounds**

You can simply ignore it. Maybe a solution [here](https://robservatory.com/month-13-is-out-of-bounds/).

**What about Fink, X11 ?**

Fink seems to be not needed anymore. `X11` has been replaced by `XQuartz` see [here](https://support.apple.com/fr-fr/HT201341).
