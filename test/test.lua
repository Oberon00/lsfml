lsfml = require 'lsfml'

print('lsfml:', lsfml)
print('version:     ', lsfml.version)
print('sfml_version:', lsfml.sfml_version)

system = require 'lsfml.system'
print('system:', system)

print()
print('system.Time:')
for k, v in pairs(system.Time) do
    print(k, v)
end

print(system.seconds(2) + system.milliseconds(314))

debug.debug()
