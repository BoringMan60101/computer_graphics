//Use Ctrl+Shift+P and write a command "Show glslCanvas" for live preview a fragment shaders (VSCodium)
#ifdef GL_ES
    precision mediump float;
#endif

uniform vec2 u_resolution;

float X_of_y(float y)
{
    //y = y - 1.0/5.0; //сдвиг для учёта соотношения строн (пропуск пустых 'y')
    float n = 0.0;
    const float L = 0.06; //длина одного участка монотонности (половина треугольника)
    const float parts = 1.0 / L; //количество треугольников
    float angle = 7.5/3.0; //угол влияет на высоту треугольника
    for(float n = 0.0; n <= parts - 2.0; n += 2.0)
    {
        if(y > n*L && y < n*L + L)
            return angle * (y - L*n);
        if(y >= L + n*L && y <= n*L + 2.0*L)
            return angle * (L*(2.0 + n) - y);
    }
    return 2.0;
}

vec4 BahrainFlag(vec2 position)
{
    vec4 color = vec4(218.0/255.0, 41.0/255.0, 28.0/255.0, 1.0); //всё в красный сначала
    
    //отрисовка "пилы"
    float x0 = 0.2;
    if (position.x <= x0 + X_of_y(position.y - 0.2)) //учёт сдвига по 'y' для аргумента функции X(y)
        color = vec4(1.0, 1.0, 1.0, 1.0);
    
    //"удаление" части изображения для достижения соотношения сторон 3:5
    if(position.y <= 1.0/5.0  || position.y >= 4.0/5.0)
        color = vec4(0.0, 0.0, 0.0, 1.0);

    return color;

}



void main()
{
    vec2 position = gl_FragCoord.xy/u_resolution.xy;
    gl_FragColor = BahrainFlag(position);
}

