-- Inserção de dados na tabela Notifications
INSERT INTO Notifications (name, description, alert_type, condition, temperature_min, temperature_max, humidity_min, humidity_max, uv_index)
VALUES 
  ('Clima Seco', 'A umidade está baixa. Beba muita água para se manter hidratado.', 'Umidade', 'Low Humidity', NULL, NULL, 0, 20, NULL),
  ('Protetor Solar Necessário', 'O índice UV está elevado. Passe protetor solar.', 'UV', 'UV Index Elevated', NULL, NULL, NULL, NULL, 6);
