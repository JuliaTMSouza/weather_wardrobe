-- Inserir dados na tabela 'Outfits' com diferentes opções para cada período do dia

INSERT INTO Outfits (name, description, min_temperature, max_temperature, min_humidity, max_humidity, part_of_day) VALUES
-- Manhã
('Camisa de Manga Longa de Algodão e Calça de Alfaiataria', 
 'Ideal para manhãs frescas, perfeita para um dia de trabalho em que você precisa estar elegante e confortável. A calça de alfaiataria garante um visual profissional.', 
 10, 15, 40, 60, 'Manhã'),

('Blusa de Manga Longa e Saia Midi', 
 'Para manhãs levemente frescas e úmidas, ideal para um café da manhã ou uma reunião matinal. A saia midi adiciona um toque de sofisticação.', 
 15, 20, 50, 70, 'Manhã'),

('Camisa Polo e Calça Jeans', 
 'Para manhãs amenas, confortável e casual. Ideal para atividades do dia a dia ou para ir ao trabalho em um ambiente mais descontraído.', 
 20, 25, 60, 80, 'Manhã'),

-- Tarde
('Camiseta de Algodão e Shorts de Linho', 
 'Perfeito para dias quentes e secos, este look é confortável e respirável, ideal para atividades ao ar livre ou um dia descontraído em casa.', 
 25, 30, 20, 50, 'Tarde'),

('Blusa de Manga Curta de Viscose e Calça Leve de Sarja', 
 'Para dias quentes e um pouco úmidos, essa combinação é leve e fresca. Ideal para um almoço com amigos ou um passeio durante a tarde.', 
 30, 35, 50, 70, 'Tarde'),

('Camisa de Manga Curta e Bermuda de Tecido', 
 'Para tardes quentes e úmidas, confortável e adequado para um passeio ou um evento social.', 
 28, 33, 60, 80, 'Tarde'),

-- Noite
('Suéter de Lã e Calças de Moletom', 
 'Ideal para noites frescas, esse look é confortável e quente. Perfeito para uma noite relaxante em casa ou uma caminhada leve após o jantar.', 
 10, 15, 30, 60, 'Noite'),

('Jaqueta Leve e Calças de Veludo', 
 'Para noites amenas e um pouco úmidas, oferece uma camada extra de conforto. Ideal para uma saída casual ou um evento social à noite.', 
 15, 20, 50, 70, 'Noite'),

('Jaqueta Pesada e Botas de Couro', 
 'Para noites frias e úmidas, essa combinação oferece proteção extra contra o frio. Ideal para sair em uma noite de inverno ou para um evento noturno ao ar livre.', 
 0, 10, 70, 90, 'Noite');
