cd build/
cmake ../
if make ; then
    ./opengl-engine
else
    echo "Build failed."
fi

