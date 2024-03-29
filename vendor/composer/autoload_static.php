<?php

// autoload_static.php @generated by Composer

namespace Composer\Autoload;

class ComposerStaticInit5b443e7bc2dfdc56c138efd20aec74c3
{
    public static $prefixLengthsPsr4 = array (
        'D' => 
        array (
            'DiDom\\' => 6,
        ),
    );

    public static $prefixDirsPsr4 = array (
        'DiDom\\' => 
        array (
            0 => __DIR__ . '/..' . '/imangazaliev/didom/src/DiDom',
        ),
    );

    public static $classMap = array (
        'Composer\\InstalledVersions' => __DIR__ . '/..' . '/composer/InstalledVersions.php',
    );

    public static function getInitializer(ClassLoader $loader)
    {
        return \Closure::bind(function () use ($loader) {
            $loader->prefixLengthsPsr4 = ComposerStaticInit5b443e7bc2dfdc56c138efd20aec74c3::$prefixLengthsPsr4;
            $loader->prefixDirsPsr4 = ComposerStaticInit5b443e7bc2dfdc56c138efd20aec74c3::$prefixDirsPsr4;
            $loader->classMap = ComposerStaticInit5b443e7bc2dfdc56c138efd20aec74c3::$classMap;

        }, null, ClassLoader::class);
    }
}
